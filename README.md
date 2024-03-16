## Contents
* [autoEndTask](#autoendtask)
* [oldMenu](#oldmenu)
* [Powershell commands](#powershell-commands)

## Description
A class that offers methods to checks a single number to see if it is a prime number or returns a list of prime numbers in a certain range. Depending on the range, instead of checking each number separately, [sieve of Eratosthenes](https://de.wikipedia.org/wiki/Sieb_des_Eratosthenes) is used for acceleration.

The sieve field is compressed by a [2,3,5-wheel factorization](https://en.wikipedia.org/wiki/Wheel_factorization) so that there are 8 possible primes in each range of the 30 numbers, whose offsets are encoded into a corresponding unsigned 8-bit integer. By reducing the sieve field in size, the processor can achieve a higher data density in its internal caches, which are far faster than a ram stick.

If the [sieve of Eratosthenes](https://de.wikipedia.org/wiki/Sieb_des_Eratosthenes) is not used, each [2,5-wheel factorized](https://en.wikipedia.org/wiki/Wheel_factorization) number is checked instead to see whether it is a prime number. This happens asynchronously and scales by itself. Numbers at the beginning of the [unsigned 64-bit integer](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-dtyp/a7b7720f-87eb-4add-9bcb-c6ff652778ae) spectrum can be checked faster than numbers at the end of the [unsigned 64-bit integer](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-dtyp/a7b7720f-87eb-4add-9bcb-c6ff652778ae) spectrum. To avoid waiting times of the [asynchronous threads](https://en.cppreference.com/w/cpp/thread/async) to be supplied with the next queues, each queue is scaled depending on the current segmented range and the number of [asynchronous threads](https://en.cppreference.com/w/cpp/thread/async), so the main thread can keep up and supply next queues intime. If the range is short and the numbers are low, the additional overhead of initializing many asynchronous threads is avoided by simply using fewer threads.

## Why did i made this
I wanted to practice [asynchronous threading](https://en.cppreference.com/w/cpp/thread/async), C++ and deploying a standalone executable that would run on any 64bit Windows machine.

## Optional progress tracker
On creation as new object or afterwards via a method, the optional progress tracker can be enabled or disabled. Enabling has an impact on performance and tracks linear to the given range.

## Demo
This view shows the terminal view with an excerpt from the Task Manager, which are simultaneously recorded by OBS:

![demonstration terminal & task manager](https://github.com/AlparDuman/primes-cpp/blob/main/demonstration.gif)

Want to try for yourself? Get the it [here](https://github.com/AlparDuman/primes-cpp/releases).
