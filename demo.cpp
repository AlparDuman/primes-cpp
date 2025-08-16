/*
    Copyright (C) 2024 Alpar Duman
    This file is part of primes-cpp.

    video-converter-javascript is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3 as
    published by the Free Software Foundation.

    video-converter-javascript is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with video-converter-javascript. If not, see
    <https://github.com/AlparDuman/primes-cpp/blob/main/LICENSE>
    else <https://www.gnu.org/licenses/>.
*/

#include "primes.cpp"
#include <future>
#include <iostream>
#include <string>
#include <stdlib.h>

int main()
{
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::duration<float> fsec;
    fsec fs;

    std::vector<uint64_t> listPrimes;
    std::future<std::vector<uint64_t>> asyncPrimes;

    auto primes = new Primes;
    float progress, progressDisplay;
    uint64_t low, high, listPrimesSize;
    std::string selectedMethod;
    char showProgress = 'y';
    auto t0 = Time::now(), t1 = Time::now();

    std::cout << "      ___         ___                       ___           ___           ___     " << std::endl;
    std::cout << "     /  /\\       /  /\\        ___          /__/\\         /  /\\         /  /\\    " << std::endl;
    std::cout << "    /  /::\\     /  /::\\      /  /\\        |  |::\\       /  /:/_       /  /:/_   " << std::endl;
    std::cout << "   /  /:/\\:\\   /  /:/\\:\\    /  /:/        |  |:|:\\     /  /:/ /\\     /  /:/ /\\  " << std::endl;
    std::cout << "  /  /:/~/:/  /  /:/~/:/   /__/::\\      __|__|:|\\:\\   /  /:/ /:/_   /  /:/ /::\\ " << std::endl;
    std::cout << " /__/:/ /:/  /__/:/ /:/___ \\__\\/\\:\\__  /__/::::| \\:\\ /__/:/ /:/ /\\ /__/:/ /:/\\:\\" << std::endl;
    std::cout << " \\  \\:\\/:/   \\  \\:\\/:::::/    \\  \\:\\/\\ \\  \\:\\~~\\__\\/ \\  \\:\\/:/ /:/ \\  \\:\\/:/~/:/" << std::endl;
    std::cout << "  \\  \\::/     \\  \\::/~~~~      \\__\\::/  \\  \\:\\        \\  \\::/ /:/   \\  \\::/ /:/ " << std::endl;
    std::cout << "   \\  \\:\\      \\  \\:\\          /__/:/    \\  \\:\\        \\  \\:\\/:/     \\__\\/ /:/  " << std::endl;
    std::cout << "    \\  \\:\\      \\  \\:\\         \\__\\/      \\  \\:\\        \\  \\::/        /__/:/   " << std::endl;
    std::cout << "     \\__\\/       \\__\\/                     \\__\\/         \\__\\/         \\__\\/    " << std::endl;
    std::cout << std::endl << " v1.1 | GPL-3.0 license | https://github.com/alparduman | Press [enter] to start";
    std::cin.ignore();
    
    system("cls");
    std::cout << std::endl << std::endl;

    while (true)
    {
        std::cout << "Search primes from [0.." << UINT64_MAX << "]: ";
        std::cin >> low;
        std::cout << "Search primes  to  [0.." << UINT64_MAX << "]: ";
        std::cin >> high;
        std::cout << "Select a mode          [Sieve|Interval|Both]: ";
        std::cin >> selectedMethod;
        std::cout << "Show realtime progress                 [y|n]: ";
        std::cin >> showProgress;

        if (showProgress == 'y')
            primes->doProgressTracking(true);
        else
            primes->doProgressTracking(false);

        // No mode selected

        if (selectedMethod != "Sieve" && selectedMethod != "Interval" && selectedMethod != "Both")
            std::cout << std::endl
                      << "[Please select a method]" << std::endl;

        // Demo sieve

        if (selectedMethod == "Sieve" || selectedMethod == "Both")
        {
            std::cout << std::endl
                      << "[Sieve]" << std::endl;

            t0 = Time::now();
            asyncPrimes = std::async(
                std::launch::async,
                [](Primes *asyncClass, uint64_t limit)
                {
                    return asyncClass->get(limit);
                },
                std::ref(primes),
                high > low ? high : low);

            if (showProgress == 'y')
            {
                progressDisplay = -1;
                do
                {
                    progress = (uint16_t)(primes->getProgress() * 1000);
                    if (progressDisplay < progress / 10)
                    {
                        progressDisplay = progress / 10;
                        if (progressDisplay < 10)
                            printf("\r    %.1f%%", progressDisplay);
                        else
                            printf("\r   %.1f%%", progressDisplay);
                    }
                } while (asyncPrimes.wait_for(std::chrono::seconds(0)) != std::future_status::ready);
                std::cout << "\r  100.0%  " << std::endl;
            }

            listPrimes = asyncPrimes.get();
            t1 = Time::now();
            fs = t1 - t0;

            listPrimesSize = listPrimes.size();
            if (listPrimesSize < 1)
                std::cout << "  Found no primes in " << fs.count() << "s" << std::endl;
            else if (listPrimesSize < 2)
                std::cout << "  Found 1 prime in " << fs.count() << "s" << std::endl;
            else
                std::cout << "  Found " << listPrimes.size() << " primes in " << fs.count() << "s" << std::endl;
        }

        // Demo interval

        if (selectedMethod == "Interval" || selectedMethod == "Both")
        {
            std::cout << std::endl
                      << "[Interval]" << std::endl;

            t0 = Time::now();
            asyncPrimes = std::async(
                std::launch::async,
                [](Primes *asyncClass, uint64_t low, uint64_t high)
                {
                    // For demo force lower limit to be atleast first prime
                    // This is for demonstration in Both mode, when lower limit is set below 3
                    // But get() method would select sieving instead
                    if ((high > low ? high : low) < 3)
                        return asyncClass->get(0, 0);
                    else
                        return asyncClass->get(3, high > low ? high : low);
                },
                std::ref(primes),
                low,
                high);

            if (showProgress == 'y')
            {
                progressDisplay = -1;
                do
                {
                    progress = (uint16_t)(primes->getProgress() * 1000);
                    if (progressDisplay < progress / 10)
                    {
                        progressDisplay = progress / 10;
                        if (progressDisplay < 10)
                            printf("\r    %.1f%%", progressDisplay);
                        else
                            printf("\r   %.1f%%", progressDisplay);
                    }
                } while (asyncPrimes.wait_for(std::chrono::seconds(0)) != std::future_status::ready);
                std::cout << "\r  100.0%  " << std::endl;
            }

            listPrimes = asyncPrimes.get();
            t1 = Time::now();
            fs = t1 - t0;

            listPrimesSize = listPrimes.size();
            if (listPrimesSize < 1)
                std::cout << "  Found no primes in " << fs.count() << "s" << std::endl;
            else if (listPrimesSize < 2)
                std::cout << "  Found 1 prime in " << fs.count() << "s" << std::endl;
            else
                std::cout << "  Found " << listPrimes.size() << " primes in " << fs.count() << "s" << std::endl;
        }

        // Padding top for next loop

        std::cout << std::endl;
    }
    return 0;
}

