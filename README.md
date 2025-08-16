# Primes C++

Generate primes in cpp, thats all :)

| Table of Contents |
| - |
| [Description](#description) |
| [Demonstration](#demo) |

## Description
<p align="justify">A class that offers methods to check a single number to see if it is a prime number or returns a list of prime numbers in a certain range. Depending on the range, instead of checking each number separately, <a href="https://de.wikipedia.org/wiki/Sieb_des_Eratosthenes">sieve of Eratosthenes</a> is used for acceleration.</p>

<p align="justify">The sieve field is compressed by a <a href="https://en.wikipedia.org/wiki/Wheel_factorization">2,3,5-wheel factorization</a> so that there are 8 possible primes in each range of the 30 numbers, whose offsets are encoded into a corresponding <a href="https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-dtyp/a88ed362-a905-4ed2-85f5-cfc8692c9842">unsigned 8-bit integer</a>. By reducing the sieve field in size, the processor can achieve a higher data density in its internal caches, which results in faster calculation time.</p>

<p align="justify">If the sieve of Eratosthenes is not used, each <a href="https://en.wikipedia.org/wiki/Wheel_factorization">2,5-wheel factorized</a> number is checked instead to see whether it is a prime number. This happens concurrently and scales by itself. Numbers at the beginning of the <a href="https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-dtyp/a7b7720f-87eb-4add-9bcb-c6ff652778ae">unsigned 64-bit integer</a> spectrum can be checked faster than numbers at the end of the spectrum. To avoid waiting times of the <a href="https://en.cppreference.com/w/cpp/thread/async">asynchronous threads</a> to be supplied with the next queues, each queue is scaled depending on the current segmented range and the number of asynchronous threads, so the main thread can keep up and supply next queues intime. If the range is short and the numbers are low, the additional overhead of initializing many asynchronous threads is avoided by simply using fewer threads.</p>

<p align="justify">On creation as new object or afterwards via a method, the optional progress tracker can be enabled or disabled. Enabling has an impact on performance and tracks linear to the given range.</p>

## Demonstration
<p align="justify">Test run without progress tracking on <a href="https://ark.intel.com/content/www/de/de/ark/products/212047/intel-core-i7-11700k-processor-16m-cache-up-to-5-00-ghz.html">i7 11700k</a> (default cpu uefi settings). The result of the interval method is 1 less, because it has to skip the first prime number 2 on initialization, else the get() method would use sieve of Eratosthenes instead.</p>

![demonstration terminal & task manager](https://github.com/AlparDuman/primes-cpp/blob/main/test.png)
