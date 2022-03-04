#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <thread>
#include <vector>


using namespace std;


/**
 * Number of threads
 */
int NUM_THREADS = 50;


/**
 * Data blocks
 */
vector<string> dataBlocks;


/**
 * Compute diff time
 *
 * @param startTime start moment
 * @return time difference in milliseconds
 */
template <class T>
double diffTime(const chrono::time_point<T> & startTime) {
    auto endTime = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
}


/**
 * Loads the entire data and breaks it into blocks.
 */
void loadData(int numBlocks) {

    // Measure elapsed time
    auto startTime = std::chrono::steady_clock::now();

    // Open the input file and get the fileSize data
    ifstream file("./data/shakespeare.txt", ios::ate);
    streamsize fileSize = file.tellg();
    if (fileSize == -1) {
        perror("Failed open the file\n");
        exit(EXIT_FAILURE);
    }

    file.seekg(0, ios::beg);
    printf("File opened with %td bytes\n", fileSize);

    // Load data from file
    vector<char> buffer(fileSize);
    if (!file.read(buffer.data(), fileSize)) {
        perror("[ERROR] Failed to read the input file\n");
        exit(EXIT_FAILURE);
    }
    printf("Data loaded into buffer\n");

    // Size the input
    dataBlocks.resize(numBlocks);

    // Break data into blocks
    int blockSize = int(fileSize) / numBlocks;
    int blockStart = 0;
    int blockEnd = blockSize;
    for (int blockInx=0; blockInx < numBlocks; blockInx++) {

        // Look for the first character space in order to avoid splitting a word
        while (blockEnd < fileSize && buffer[blockEnd] != ' ') {
            blockEnd++;
        }

        // Create the block
        dataBlocks[blockInx] = string(&buffer[blockStart], blockEnd - blockStart);
        printf("Created block #%d from %d to %d with %d bytes\n", blockInx, blockStart, blockEnd, blockEnd-blockStart);

        // Update indexes
        blockStart = blockEnd + 1;
        blockEnd = blockStart + blockSize;
        if (blockStart > fileSize) {
            break;
        } else if (blockEnd > fileSize) {
            blockEnd = fileSize;
        }
    }
    printf("All blocks were created\n");

    // Compute elapsed time
    printf("The load step took %f milliseconds.\n", diffTime(startTime));
    std::cout << std::flush;
}


/**
 * Executes the search
 */
void execute() {

    // Measure elapsed time
    auto startTime = std::chrono::steady_clock::now();

    printf("Starting the execution with %d threads\n", NUM_THREADS);

    // <YOUR CODE>

    // Compute elapsed time
    printf("The execution step took %f milliseconds.\n", diffTime(startTime));
    std::cout << std::flush;
}


/**
 * Process the shakespeare book collection and count occurrences of the words love and hate.
 */
int main() {

    // Measure elapsed time
    // auto startTime = std::chrono::steady_clock::now();

    // Load the data
    loadData(NUM_THREADS);

    // Execute the search
    // execute();

    // Compute elapsed time
    // printf("The total time was %f milliseconds.\n", diffTime(startTime));
    // std::cout << std::flush;

    return EXIT_SUCCESS;
}
