// Repository https://github.com/AlparDuman/primes-cpp
// GNU General Public License v3.0 https://github.com/AlparDuman/primes-cpp/blob/main/LICENSE

#pragma once

#include <array>
#include <cmath>
#include <future>
#include <vector>

class Primes
{
public:
    Primes(bool doProgressTracking = false);
    ~Primes();

    const uint64_t countCores = std::thread::hardware_concurrency();

    // Enable progress tracking
    void doProgressTracking(bool enable = false);

    // Get progress
    long double getProgress();

    // Testing whether a number is prime
    inline bool isPrime(uint64_t number) const;

    // Get a list of prime numbers in the range
    const std::vector<uint64_t> &get(uint64_t startRange = 0, uint64_t endRange = 0);

private:
    std::vector<uint64_t> primesList;
    const uint64_t intervalQueueBaseSize = UINT16_MAX * countCores;
    long double progress = 0;
    bool showProgress;

    // Set to next prime candidate
    void nextPrimeCandidate(uint64_t &number);

    // Generate prime numbers up to the limit
    void sieves(uint64_t limit = 0);

    // Generating prime numbers in the range
    void interval(uint64_t startRange = 0, uint64_t endRange = 0);

    // Convert offset to bit position, otherwise return 0
    inline uint8_t numberToBit(uint64_t num) const;
};
