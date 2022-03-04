#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>


using namespace std;


/**
 * Number of threads
 */
int NUM_THREADS = 5;


/**
 * All letters of the alphabet
 */
std::string ALPHABET_LETTERS = "abcdefghijklmnopqrstuvwxyz";

/**
 * Data blocks
 */
vector<string> dataBlocks;


/**
 * Love count array
 */
vector<int> loveCount(NUM_THREADS, 0);

/**
 * Hate count array
 */
vector<int> hateCount(NUM_THREADS, 0);


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

bool didWordEnd(char nextLetter){
    // Next char should not be a letter
    for (char l : ALPHABET_LETTERS){
        if (nextLetter == l){
            return false;
        }
    }
    return true;
}

/** Function to be passed to each thread
 * For now, it only counts love words
 */
void countHateLoveIn(string text, int threadInx){
    
    // Retrieve thread ID for loging purposes
    std::ostringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    std::string threadId = threadIdStream.str();

    int endPos = text.size();

    const std::string loveWord = "love";
    const std::string hateWord = "hate";

    // // Indexes used to count each word
    // int loveIndex = 0;
    // int hateIndex = 0;

    int currentPos = 0;

    while (currentPos < endPos) {

        if (text[currentPos] == loveWord[0]) {
            int loveIndex = 0;
            while(currentPos < endPos && text[currentPos] == loveWord[loveIndex]) {
                loveIndex++;
                currentPos++;
                if (didWordEnd(text[currentPos]) && loveIndex > 3) {
                    loveCount[threadInx]++;
                    break;
                }
            }
        } else if (text[currentPos] == hateWord[0]) {
            int hateIndex = 0;
            while(currentPos < endPos && text[currentPos] == hateWord[hateIndex]) {
                hateIndex++;
                currentPos++;
                if (didWordEnd(text[currentPos]) && hateIndex > 3) {
                    hateCount[threadInx]++;
                    break;
                }
            }
        } else {
            currentPos++;
        }
    }

    // while (currentPos < endPos){
    //     if (text[currentPos] == loveWord[loveIndex]){
    //         loveIndex++;
    //         if (didWordEnd(text[currentPos+1]) && loveIndex == 3){
    //             loveCount[threadInx]++;
    //             loveIndex = 0;
    //         }
    //     }
    //     // if (lettersMatch(currentPos, loveIndex, loveWord)){
    //     //     loveIndex++;
    //     //     if (didWordEnd(currentPos) && loveIndex == 3){
    //     //         loveCount[threadId]++;
    //     //         loveIndex = 0;
    //     //     }
    //     // }
    //     else {
    //         loveIndex = 0;
    //     }
    //     currentPos++;
    // }

    printf("[%s] Found %i love words and %i hate words.\n", threadId.c_str(), loveCount[threadInx], hateCount[threadInx]);
    std::cout << std::flush;
}

/**
 * Executes the search
 */
void execute() {

    // Measure elapsed time
    auto startTime = std::chrono::steady_clock::now();

    printf("Starting the execution with %d threads\n", NUM_THREADS);

    vector<thread*> threadList;
    for (int threadInx=0; threadInx < NUM_THREADS; threadInx++) {
        threadList.push_back(new thread(countHateLoveIn, dataBlocks[threadInx], threadInx));
    }

    for (thread * thread : threadList) {
        thread->join();
        delete thread;
    }

    // Compute elapsed time
    printf("The execution step took %f milliseconds.\n", diffTime(startTime));
    std::cout << std::flush;
}


/**
 * Process the shakespeare book collection and count occurrences of the words love and hate.
 */
int main() {

    // Measure elapsed time
    auto startTime = std::chrono::steady_clock::now();

    // Load the data
    loadData(NUM_THREADS);

    // Execute the search
    execute();

    // Compute elapsed time
    printf("The total time was %f milliseconds.\n", diffTime(startTime));
    std::cout << std::flush;

    // Count total
    int loveTotal = 0;
    for (int numLoves : loveCount) {
        loveTotal += numLoves;
    }

    int hateTotal = 0;
    for (int numHates : hateCount) {
        hateTotal += numHates;
    }
    printf("Found a total of %i love words and %i hate words.\n", loveTotal, hateTotal);

    return EXIT_SUCCESS;
}
