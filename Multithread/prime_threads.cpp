#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>

std::ofstream outFile("primes.dat");
std::mutex mtx;
long long sharedCounter = 2; // Started checking from 2
long long totalPrimes = 0;   // Total number of primes found
const long long MAX_NUM = 1000000;

// Function to check if a number is prime
bool isPrime(long long n) {
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (long long i = 3; i <= n / 2; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// Thread function to find primes
void findPrimes(int threadId, long long& threadPrimeCount) {
    threadPrimeCount = 0;
    while (true) {
        long long num;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (sharedCounter > MAX_NUM) {
                return; 
            }
            num = sharedCounter++;
        }
        if (isPrime(num)) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                outFile << num << "\n";
                totalPrimes++;
                threadPrimeCount++;
            }
        }
    }
}

int main() {
    if (!outFile.is_open()) {
        std::cerr << "Error opening primes.dat" << std::endl;
        return 1;
    }

    // Created 4 threads
    std::vector<std::thread> threads;
    std::vector<long long> threadPrimeCounts(4, 0); // Per-thread prime counts
    for (int i = 0; i < 4; i++) {
        threads.emplace_back(findPrimes, i, std::ref(threadPrimeCounts[i]));
    }

    // Joined threads
    for (auto& t : threads) {
        t.join();
    }

    // Closing the file
    outFile.close();

    // Report results
    std::cout << "Total number of primes found: " << totalPrimes << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << "Thread " << i << " found " << threadPrimeCounts[i] << " primes" << std::endl;
    }

    return 0;
}