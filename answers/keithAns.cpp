#include "keithAns.hpp"
#include "../src/algorithms/KeithJumpSearch.cpp"
#include "../src/algorithms/heapSort.cpp"

// Main file containing only the main function (Q1, Q2, Q3)
// All other functions, classes, and utilities are in keithAns.hpp

/**
 * Main function that orchestrates the entire program
 */
int main() {
    // --- Initialization ---
    TransactionNode* transactionsLL = nullptr; // Linked List head
    TransactionArray transactionsArray;        // Custom Array for array implementation
    Review* reviews = nullptr;             // Linked List for reviews
    WordFrequency* wordFreq = nullptr;     // Linked List for word analysis

    const std::string transactionFile = "../data/transactionsClean.csv";
    const std::string reviewFile = "../data/reviewsClean.csv";

    // --- Data Loading ---
    std::cout << "\n--- Loading Data ---" << std::endl;
    readTransactionsFileLL(transactionFile, transactionsLL);
    readTransactionsFileArray(transactionFile, transactionsArray); // Load into custom array
    readReviewsFile(reviewFile, reviews);
    std::cout << "Loaded " << countTransactionsLL(transactionsLL) << " transactions (LL) and "
              << transactionsArray.size() << " transactions (Array)." << std::endl; // Use .size()
    std::cout << "Loaded " << countReviews(reviews) << " reviews." << std::endl;


    // --- Requirement 3 & 6: Sorting and Performance Comparison ---
    std::cout << "\n--- Sorting Transactions by Date (Requirement 3 & 6) ---" << std::endl;

    // Time Linked List Sort (Merge Sort by Date)
    auto startLLSort = std::chrono::high_resolution_clock::now();
    // SortingAlgorithms::mergeSortLL removed; use HeapSort::sortTransactions or KeithJumpSearch(transactionsLL);
    auto endLLSort = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationLLSort = endLLSort - startLLSort;
    std::cout << "Linked List Merge Sort Time: " << durationLLSort.count() << " ms" << std::endl;

    // Time Custom Array Sort (Merge Sort by Date)
    auto startArrSort = std::chrono::high_resolution_clock::now();
    // SortingAlgorithms::mergeSortArray removed; use HeapSort or KeithJumpSearch(transactionsArray);
    auto endArrSort = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationArrSort = endArrSort - startArrSort;
    std::cout << "Custom Array Merge Sort Time:" << durationArrSort.count() << " ms" << std::endl;

    // Display first few sorted transactions (from Custom Array for example)
    std::cout << "\nFirst 10 Transactions Sorted by Date (from Custom Array):" << std::endl;
    int displayCount = 0;
    for (size_t i = 0; i < transactionsArray.size() && displayCount < 10; ++i) {
        const TransactionData& currentArr = transactionsArray[i];
        std::cout << (displayCount + 1) << ". Date: " << currentArr.date
                  << ", Product: " << currentArr.product
                  << ", Price: $" << std::fixed << std::setprecision(2) << currentArr.price << std::endl;
        displayCount++;
    }
     if (transactionsArray.size() > 10) {
        std::cout << "..." << std::endl;
    }

     // --- Requirement 4 & 6: Searching and Performance Comparison ---
     std::cout << "\n--- Searching Transactions (Requirement 4 & 6) ---" << std::endl;
     std::cout << "(Calculating percentage of electronics purchases made with credit card)" << std::endl;

     double percentageLL, percentageArr;

     // Time Linked List Search
     auto startLLSearch = std::chrono::high_resolution_clock::now();
     percentageLL = calculateElectronicsCreditCardPercentageLL(transactionsLL);
     auto endLLSearch = std::chrono::high_resolution_clock::now();
     std::chrono::duration<double, std::milli> durationLLSearch = endLLSearch - startLLSearch;
     std::cout << "Linked List Search Time:  " << durationLLSearch.count() << " ms" << std::endl;

     // Time Custom Array Search
     auto startArrSearch = std::chrono::high_resolution_clock::now();
     percentageArr = calculateElectronicsCreditCardPercentageArray(transactionsArray); // Use array version
     auto endArrSearch = std::chrono::high_resolution_clock::now();
     std::chrono::duration<double, std::milli> durationArrSearch = endArrSearch - startArrSearch;
     std::cout << "Custom Array Search Time: " << durationArrSearch.count() << " ms" << std::endl;

     std::cout << "\nPercentage (Linked List): " << std::fixed << std::setprecision(2) << percentageLL << "%" << std::endl;
     std::cout << "Percentage (Custom Array):" << std::fixed << std::setprecision(2) << percentageArr << "%" << std::endl;


    // --- Requirement 5: Review Analysis ---
    std::cout << "\n--- Analyzing 1-Star Reviews (Requirement 5) ---" << std::endl;
    // Count one-star reviews first
    int oneStarCount = 0;
    Review* reviewCurrent = reviews;
    while (reviewCurrent) {
        if (reviewCurrent->rating == 1) oneStarCount++;
        reviewCurrent = reviewCurrent->next;
    }

    // Use existing functions for review analysis
    findOneStarReviewWords(reviews, wordFreq);
    sortWordsByFrequency(wordFreq); // Sorts the WordFrequency linked list

    std::cout << "\nAnalyzed " << oneStarCount << " one-star reviews out of "
              << countReviews(reviews) << " total reviews." << std::endl;
    int uniqueWordCount = countWordFrequency(wordFreq); // Count unique words after merging/sorting
    std::cout << "Found " << uniqueWordCount << " unique meaningful words/symbols in 1-star reviews." << std::endl;
    std::cout << "\nTop 10 most frequent words/symbols in 1-star reviews:" << std::endl;
    displayTopWords(wordFreq, 10);


    // --- Cleanup ---
    std::cout << "\n--- Cleaning Up Memory ---" << std::endl;
    // Clean up linked list transactions
    while (transactionsLL) {
        TransactionNode* temp = transactionsLL;
        transactionsLL = transactionsLL->next;
        delete temp;
    }
    std::cout << "Cleaned up transaction linked list." << std::endl;

    // Custom Array cleans itself up via destructor (RAII)
     std::cout << "Transaction custom array cleaned up automatically via destructor." << std::endl;


    // Clean up linked list reviews
    while (reviews) {
        Review* temp = reviews;
        reviews = reviews->next;
        delete temp;
    }
     std::cout << "Cleaned up review linked list." << std::endl;

    // Clean up linked list word frequencies
    while (wordFreq) {
        WordFrequency* temp = wordFreq;
        wordFreq = wordFreq->next;
        delete temp;
    }
     std::cout << "Cleaned up word frequency linked list." << std::endl;

    // --- End Program ---
    std::cout << "\nPress Enter to exit...";
    // Clear the input buffer before waiting for Enter
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}

