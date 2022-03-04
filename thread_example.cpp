#include <chrono>
#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>


/**
 * Max limit for our example
 */
const long int MAX_PRIME_TARGET = 100000;


/**
 * Number of runs to be executed for one measurement
 */
const int NUM_RUNS = 10;


/**
 * Checks if a given number is prime.
 *
 * @param n the number to be checked
 * @return True if is prime
 */
bool isPrime(long int n) {

    // Enforce 0 and 1 are not prime numbers
    if (n == 0 || n == 1) {
        return false;
    }

    // Check if the number is prime
    for (int inx = 2; inx < n; inx++) {

        // If the number is divisible by inx then it is not prime
        if ((n % inx) == 0) {
            return false;
        }
    }

    // The number is prime
    return true;
}


/**
 * Assess how many prime numbers exist between 0 and 10^5
 */
void run() {
    // Check how many prime numbers exist between 0 and 10^5
    int numPrimes = 0;
    for (long int i=0; i < MAX_PRIME_TARGET; i++) {

        if ((i % 1000) == 0) {
            printf(".");
            std::cout << std::flush;
        }

        if (isPrime(i)) {
            //printf("The number '%d' is prime.\n", i);
            numPrimes++;
        }
    }
    printf("\n");
    printf("NUM_PRIMES: %d\n", numPrimes);
}


/**
 *  Assess how many prime numbers exist between 0 and 10ˆ5; using multiple processes to compute.
 *
 * @param numProcesses number of processes to spawn.
 */
void runMultiProcess(int numProcesses) {

    // Create a process for each block
    pid_t pid;
    bool isParent = true;
    int processInx = 0;
    while (processInx < numProcesses) {

        // Fork the current process
        pid = fork();

        // Handle failure
        if (pid == -1) {
            printf("Failed to fork process\n");
            exit(EXIT_FAILURE);

        // Handle parent case
        } else if (pid > 0) {
            printf("[%d] Parent\n", getpid());
            processInx++;

        // Handle child case
        } else {
            pid = getpid();
            printf("[%d] Child\n", pid);

            // Set the parent flag
            isParent = false;

            // Break the loop to avoid creating more processes
            break;
        }
    }

    // If this is the parent
    if (isParent) {
        // Wait for all child processes to execute
        int status = 0;
        while (wait(&status) > 0);

        return;
    }

    // Set computing limits
    long int blockSize = MAX_PRIME_TARGET / numProcesses;
    long int blockStart = processInx * blockSize;
    long int blockEnd = blockStart + blockSize;
    printf("[%d] START: %ld | END: %ld\n", pid, blockStart, blockEnd);
    std::cout << std::flush;

    // Check how many prime numbers exist in the block
    int numPrimes = 0;
    for (long int i=blockStart; i < blockEnd; i++) {
        if (isPrime(i)) {
            numPrimes++;
        }
    }
    printf("[%d] START: %ld | END: %ld | NUM_PRIMES: %d\n", pid, blockStart, blockEnd, numPrimes);
    std::cout << std::flush;

    // Forcing an exit to avoid computing time for a child process
    exit(EXIT_SUCCESS);
}


/**
 * Thread to find primes in a given block
 *
 * @param blockStart start index
 * @param blockEnd end index
 */
void findPrimesThread(long int blockStart, long int blockEnd) {

    // Get the thread ID
    std::ostringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    std::string threadId = threadIdStream.str();

    printf("[%s] START: %ld | END: %ld\n", threadId.c_str(), blockStart, blockEnd);
    std::cout << std::flush;

    // Check how many prime numbers exist in the block
    int numPrimes = 0;
    for (long int i=blockStart; i < blockEnd; i++) {
        if (isPrime(i)) {
            numPrimes++;
        }
    }
    printf("[%s] START: %ld | END: %ld | NUM_PRIMES: %d\n", threadId.c_str(), blockStart, blockEnd, numPrimes);
    std::cout << std::flush;
}


/**
 *  Assess how many prime numbers exist between 0 and 10ˆ5; using multiple threads to compute.
 *
 * @param numThreads number of threads to use.
 */
void runMultiThread(int numThreads) {

    // Set the block size
    long int blockSize = MAX_PRIME_TARGET / numThreads;

    // Create the threads
    std::vector<std::thread*> threadList;
    for (int threadInx=0; threadInx < numThreads; threadInx++) {
        long int blockStart = threadInx * blockSize;
        long int blockEnd = blockStart + blockSize;
        threadList.push_back(new std::thread(findPrimesThread, blockStart, blockEnd));
    }

    // Wait for all threads to finish
    for (std::thread * thread : threadList) {
        // Wait for the thread to finish
        thread->join();

        // Remove the thread
        delete thread;
    }
}


/**
 * Print a section header
 *
 * @param name a string with the name of the section
 */
void printSection(const char * name) {
    printf("\n#############################################################################\n");
    printf("###  %s\n", name);
    printf("#############################################################################\n");
}


/**
 * Entry point
 */
int main() {

    // Execute several times to ger an average time
    long long elapsed = 0;
    for (int i=0; i<NUM_RUNS; i++) {

        // Measure elapsed time
        auto startTime = std::chrono::steady_clock::now();

        // Execute the computation
//         printSection("SINGLE PROCESS RUN");
//         run();
//         printSection("MANY PROCESSES RUN");
//         runMultiProcess(100);
        printSection("MANY THREADS RUN");
        runMultiThread(100);

        // Compute elapsed time
        auto endTime = std::chrono::steady_clock::now();
        elapsed += std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    }

    double averageTime = double(elapsed) / double(NUM_RUNS);
    printf("\nThe routine took %f milliseconds in average.\n", averageTime);

    return EXIT_SUCCESS;
}
