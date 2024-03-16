// Repository https://github.com/AlparDuman/primes-cpp
// GNU General Public License v3.0 https://github.com/AlparDuman/primes-cpp/blob/main/LICENSE

#include "primes.h"

Primes::Primes(bool doProgressTracking)
{
    showProgress = doProgressTracking;
};
Primes::~Primes() = default;

/**
 * Testing whether a number is prime
 *
 * @param   number  Test this number
 * @return  TRUE if prime, else FALSE
 */
inline bool Primes::isPrime(uint64_t number) const
{
    // Is less than 7, check for known prime numbers 2,3,5
    if (number < 7)
    {
        if (number == 2 || number == 3 || number == 5)
            return true;
        return false;
    }

    // Is the number even or divisible by 3 or by 5?
    if (!(number & 1) || !(number % 3) || !(number % 5))
        return false;

    // Testing with 2,3,5-wheel factorization
    const uint64_t factorLimit = std::sqrt(number) + 1;
    for (uint64_t factor = 7; factor < factorLimit; factor += 6)
    {
        if (!(number % factor))
            return false;
        if (!(number % (factor += 4)))
            return false;
        if (!(number % (factor += 2)))
            return false;
        if (!(number % (factor += 4)))
            return false;
        if (!(number % (factor += 2)))
            return false;
        if (!(number % (factor += 4)))
            return false;
        if (!(number % (factor += 6)))
            return false;
        if (!(number % (factor += 2)))
            return false;
    }

    // Is prime :D
    return true;
}

/**
 * Set to next prime candidate
 *
 * @param   number  Set to next prime candidate,
 *                  on overflow mark as 0
 */
void Primes::nextPrimeCandidate(uint64_t &number)
{
    // Is less than 5, the known prime numbers 2,3,5
    if (number < 5)
        if (number < 2)
            number = 2;
        else if (number < 3)
            number = 3;
        else
            number = 5;
    // Is greater than or equal to 5
    else
    {
        // If the number is odd, raise by 2, otherwise raise by 1,
        // except if the number would end in 5 with offset, skip 7,
        // as any number ending in 5 is divisible by 5
        uint8_t offset = 1;
        if (number & 1)
            if (number % 10 == 3)
                offset = 4;
            else
                offset = 2;
        // If the offset does not lead to an overflow, apply, otherwise set to 0 as an indicator
        if (number <= UINT64_MAX - offset)
            number += offset;
        else
            number = 0;
    }
}

/**
 * Enable progress tracking
 * @return  progress of current task
 */
void Primes::doProgressTracking(bool enable)
{
    showProgress = enable;
}

/**
 * Get progress
 * @return  progress of current task
 */
long double Primes::getProgress()
{
    return progress > 1 ? 1 : progress;
}

/**
 * Convert offset to bit position, otherwise return 0
 *
 * @param   number Determine the offset of this number
 * @return  Bit offset
 */
inline uint8_t Primes::numberToBit(uint64_t number) const
{
    // Look-up table for 2,3,5-wheel factorization in 8bit position
    static std::array<uint64_t, 30> lookup{{0, 0x1, 0, 0, 0, 0, 0, 0x2, 0, 0, 0, 0x4, 0, 0x8, 0, 0, 0, 0x10, 0, 0x20, 0, 0, 0, 0x40, 0, 0, 0, 0, 0, 0x80}};
    return lookup.at(number);
}

/**
 * Get a list of prime numbers in the range
 *
 * @param   startRange  Search from here
 * @param   endRange    Search to here
 * @return  List of primes
 */
const std::vector<uint64_t> &Primes::get(uint64_t startRange, uint64_t endRange)
{
    // Reset list (maybe future sibling methods with appening functionality?)
    primesList.clear();

    // Reordering of range limits in case of unintentionally swapped input
    uint64_t swapRange;
    if (startRange > endRange)
    {
        swapRange = startRange;
        startRange = endRange;
        endRange = swapRange;
    }

    // If the range starts at the known prime number 2 or below, it is sieved, otherwise an interval check is carried out
    if (startRange < 3)
        sieves(endRange);
    else
        interval(startRange, endRange);

    // Returns a reference to the prime numbers found
    return primesList;
}

/**
 * Generating prime numbers in the range
 *
 * @param   startRange  Search from here
 * @param   endRange    Search to here
 */
void Primes::interval(uint64_t startRange, uint64_t endRange)
{
    // If the range is below the known prime number 2, exit
    if (endRange < 2)
        return;

    // Declare variables once
    auto asyncClass = new Primes;
    std::vector<uint64_t> queueCandidates, result;
    std::vector<std::vector<uint64_t>> results;
    std::vector<std::future<std::vector<uint64_t>>> asyncResults;
    uint64_t queueSize = 0, queueMaxSize, countQueues = 0, countQueuesProcessed = 0;
    size_t primesListSize = primesList.size();
    const double doubleUint64Max = UINT64_MAX;

    // Repeat as long as the number is not marked as 0 and the number is less than or equal to the end of the range
    for (uint64_t number = startRange > 0 ? startRange : 1; number != 0 && number <= endRange;)
    {
        // Dynamically set queue size to supply asynchronous instances and avoid waiting times
        // Close to     0   , the prime number check is very fast, set queue size to base size
        // Close to 2^64 - 1, the prime number check is very slow, set queue size to 1
        if (queueMaxSize > 1)
        {
            queueMaxSize = (UINT64_MAX - number > 1 ? UINT64_MAX - number : 1) / doubleUint64Max * intervalQueueBaseSize;
            if (queueMaxSize < 1)
                queueMaxSize = 1;
        }

        // Create a queue with possible prime numbers
        for (; number != 0 && number <= endRange && queueSize < queueMaxSize; nextPrimeCandidate(number))
        {
            queueCandidates.push_back(number);
            ++queueSize;
        }

        ++countQueues;

        // Assign an asynchronous instance to check the queue for prime numbers
        asyncResults.push_back(
            std::async(
                std::launch::async,
                [](Primes *asyncClass, std::vector<uint64_t> queue)
                {
                    std::vector<uint64_t> results;
                    // Add numbers from the queue to the return list if they pass the prime number check
                    for (auto &candidate : queue)
                        if (asyncClass->isPrime(candidate))
                            results.push_back(candidate);
                    return results;
                },
                std::ref(asyncClass),
                queueCandidates));

        // Reset the queue, in preparation for the next build or to clean up when exiting
        queueCandidates.clear();
        queueSize = 0;
    }

    // Retrieve results of asynchronous instances and reserve space for the main list
    for (auto &asyncResult : asyncResults)
    {
        auto result = asyncResult.get();
        results.push_back(result);
        primesListSize += result.size();
        if (showProgress)
            progress = 0.99F * (++countQueuesProcessed) / countQueues;
    }
    primesList.reserve(primesListSize);

    // Append the results of the asynchronous instances to the main list
    for (const auto &result : results)
    {
        primesList.insert(primesList.end(), result.begin(), result.end());
    }

    if (showProgress)
        progress = 0;
}

/**
 * Generate prime numbers up to the limit
 *
 * @param   limit   Search up to here
 */
void Primes::sieves(uint64_t limit)
{
    // If the range is below the known prime number 2, exit
    if (limit < 2)
        return;

    // Declare variables once
    auto sievesFields = std::vector<uint64_t>((limit / 30) + 1, 0);
    const uint64_t numberMax = UINT64_MAX - 2;
    uint8_t mask;

    // Append known prime numbers 2,3,5 to the main list if they are within the limit
    if (limit > 1)
        primesList.push_back(2);
    if (limit > 2)
        primesList.push_back(3);
    if (limit > 4)
        primesList.push_back(5);

    // Perform sieving with 1,2,3-wheel factorization and bitpacking
    for (uint64_t number = 7; number != 0 && number <= limit; number = number < numberMax ? number + 2 : 0)
    {
        // If the number is a possible prime number and is not yet marked as a non-prime number
        mask = numberToBit(number % 30);
        if (mask && !(sievesFields[number / 30] & mask))
        {
            // Append found prime number to the main list
            primesList.push_back(number);
            // Mark multiples of a prime number as a non-prime number
            if (number <= UINT64_MAX / 2)
                for (uint64_t multiple = number * 2; multiple != 0 && multiple <= limit; multiple > UINT64_MAX - number ? 0 : multiple += number)
                    sievesFields[multiple / 30] |= numberToBit(multiple % 30);
            // Set progress
            if (showProgress)
                progress = number / (long double) limit;
        }
    }
    if (showProgress)
        progress = 0;
}
