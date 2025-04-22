#include "../include/linkedList.hpp"
#include "../include/SortingAlgorithms.hpp"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>     
#include <chrono>
#include <stdexcept>
#include <limits>

/* testing
 * HEADER/ TABLE OF CONTENT:
 * 
 * Data Structures:
 * - WordNode (line 15)           - Structure for word variations and frequencies
 * - TransactionNode (line 24)    - Structure for transaction data in linked list
 * - TransactionData (line 39)    - Structure for transaction data in array
 * - TransactionArray (line 145)  - Custom dynamic array implementation
 * 
 * Word Processing Functions:
 * - splitIntoWords (line 52)     - Splits text into words and updates frequency
 * - isSymbol (line 575)          - Checks if character is a symbol
 * - normalizeWord (line 584)     - Normalizes word by removing symbols and converting to lowercase
 * - areWordsSimilar (line 601)   - Checks if two words are similar using distance metric
 * - processText (line 621)       - Processes text to extract words and symbols
 * - mergeSimilarWords (line 706) - Merges similar words in the list
 * - displayWordFrequencies (line 736) - Displays word frequencies in descending order
 * 
 * Transaction Processing Functions:
 * - merge (line 109)             - Merges two sorted transaction lists
 * - split (line 130)             - Splits transaction list into two halves
 * - mergeSortTransactionsLL (line 145) - Merge sort for linked list transactions
 * - mergeArrays (line 233)       - Merges two sorted transaction arrays
 * - mergeSortArrayRecursive (line 281) - Recursive merge sort for transaction arrays
 * - sortTransactionsArrayByDate (line 292) - Wrapper for array merge sort
 * 
 * File I/O Functions:
 * - readTransactionsFileLL (line 300) - Reads transactions into linked list
 * - readTransactionsFileArray (line 399) - Reads transactions into custom array
 * - readReviewsFile (line 482)    - Reads reviews from file
 * 
 * Counting and Analysis Functions:
 * - countTransactionsLL (line 511) - Counts transactions in linked list
 * - countTransactionsArray (line 520) - Counts transactions in array
 * - countReviews (line 524)       - Counts reviews in list
 * - calculateElectronicsCreditCardPercentageLL (line 534) - Calculates percentage for linked list
 * - calculateElectronicsCreditCardPercentageArray (line 553) - Calculates percentage for array
 * - findOneStarReviewWords (line 765) - Processes one-star reviews
 * - sortWordsByFrequency (line 796) - Sorts words by frequency
 * - displayTopWords (line 827)    - Displays top words by frequency
 * - countWordFrequency (line 849) - Counts words in frequency list
 * 
 * Main Function:
 * - main (line 868)              - Orchestrates the entire program
 */

//break down the sentence into words, put the words into linked list, and find any and all similar words instead of using this StopWord, need to break down the sentences into what expressions (need to detect symbols)

// Structure to store word variations and their frequencies
struct WordNode {
    std::string baseWord;
    std::string variation;
    int frequency;
    WordNode* next;
    
    WordNode() : frequency(0), next(nullptr) {}
};

// Renamed Transaction to TransactionNode for clarity in linked list context
struct TransactionNode {
    std::string customerID;
    std::string product;
    std::string category;
    double price;
    std::string date; // Keep as string for sorting, could parse to date object if needed
    std::string paymentMethod;
    TransactionNode* next; // Pointer for linked list

    // Default constructor
    TransactionNode() : price(0.0), next(nullptr) {}
};

// New structure for array/vector implementation (no next pointer)
struct TransactionData {
    std::string customerID;
    std::string product;
    std::string category;
    double price;
    std::string date;
    std::string paymentMethod;

    // Default constructor
    TransactionData() : price(0.0) {}
};

// Helper function to split string into words
void splitIntoWords(const std::string& text, WordFrequency*& wordFreq) {
    std::string word;
    std::istringstream iss(text);
    
    while (iss >> word) {
        // Convert to lowercase and remove punctuation
        std::string cleanWord;
        for (char c : word) {
            if (isalpha(c)) {
                cleanWord += tolower(c);
            }
        }
        
        if (cleanWord.empty()) continue;
        
        // Update word frequency
        WordFrequency* current = wordFreq;
        bool found = false;
        
        while (current != nullptr) {
            if (current->word == cleanWord) {
                current->frequency++;
                found = true;
                break;
            }
            current = current->next;
        }
        
        if (!found) {
            WordFrequency* newWord = new WordFrequency();
            newWord->word = cleanWord;
            newWord->frequency = 1;
            newWord->next = wordFreq;
            wordFreq = newWord;
        }
    }
}

// Implementation of merge sort for transactions (Linked List by Date)
TransactionNode* merge(TransactionNode* left, TransactionNode* right) {
    if (!left) return right;
    if (!right) return left;

    TransactionNode* result = nullptr;

    // Compare dates as strings (YYYY-MM-DD format assumed)
    if (left->date <= right->date) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

void split(TransactionNode* source, TransactionNode*& left, TransactionNode*& right) {
    if (!source || !source->next) {
        left = source;
        right = nullptr;
        return;
    }

    TransactionNode* slow = source;
    TransactionNode* fast = source->next;

    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    left = source;
    right = slow->next;
    slow->next = nullptr; // Split the list
}

// Merge Sort for Linked List (by date)
void mergeSortTransactionsLL(TransactionNode*& head) {
    if (!head || !head->next) return;

    TransactionNode* left;
    TransactionNode* right;

    split(head, left, right);

    mergeSortTransactionsLL(left);
    mergeSortTransactionsLL(right);

    head = merge(left, right);
}

// --- Custom Dynamic Array Implementation for TransactionData ---
class TransactionArray {
private:
    TransactionData* data; // Pointer to the dynamically allocated array
    size_t arraySize;      // Current number of elements
    size_t arrayCapacity;  // Allocated memory capacity

    // Private helper to resize the array when capacity is reached
    void resize(size_t newCapacity) {
        if (newCapacity <= arrayCapacity) return; // Only grow

        TransactionData* newData = new TransactionData[newCapacity];
        // Copy existing elements
        for (size_t i = 0; i < arraySize; ++i) {
            newData[i] = data[i]; // Assumes TransactionData has a proper copy assignment or is POD-like
        }

        delete[] data;       // Free old memory
        data = newData;      // Point to new memory
        arrayCapacity = newCapacity;
    }

public:
    // Constructor
    TransactionArray() : data(nullptr), arraySize(0), arrayCapacity(0) {
        resize(10); // Initial capacity
    }

    // Destructor
    ~TransactionArray() {
        delete[] data;
    }

    // Add element to the end
    void push_back(const TransactionData& transaction) {
        if (arraySize == arrayCapacity) {
            resize(arrayCapacity == 0 ? 10 : arrayCapacity * 2); // Double capacity
        }
        data[arraySize++] = transaction;
    }

    // Access element by index (const version)
    const TransactionData& at(size_t index) const {
        if (index >= arraySize) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    // Access element by index (non-const version)
    TransactionData& at(size_t index) {
        if (index >= arraySize) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    // Access element by index using operator[] (const version)
     const TransactionData& operator[](size_t index) const {
         // Note: No bounds checking for performance, similar to std::vector[]
         return data[index];
     }

     // Access element by index using operator[] (non-const version)
     TransactionData& operator[](size_t index) {
         // Note: No bounds checking for performance
         return data[index];
     }


    // Get current size
    size_t size() const {
        return arraySize;
    }

    // Check if empty
    bool empty() const {
        return arraySize == 0;
    }

     // Direct access to the underlying data (use with caution)
     TransactionData* getDataPtr() { return data; }
     const TransactionData* getDataPtr() const { return data; }

};

// --- Merge Sort Implementation for Custom TransactionArray ---

// Merge function for TransactionData arrays (sorted by date)
void mergeArrays(TransactionData* arr, size_t left, size_t mid, size_t right) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    // Create temporary arrays
    TransactionData* L = new TransactionData[n1];
    TransactionData* R = new TransactionData[n2];

    // Copy data to temp arrays L[] and R[]
    for (size_t i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (size_t j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temp arrays back into arr[left..right]
    size_t i = 0; // Initial index of first subarray
    size_t j = 0; // Initial index of second subarray
    size_t k = left; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].date <= R[j].date) { // Compare dates
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

// Main Merge Sort function for TransactionData arrays (recursive)
void mergeSortArrayRecursive(TransactionData* arr, size_t left, size_t right) {
    if (left >= right) {
        return; // Base case: array of size 0 or 1 is sorted
    }
    size_t mid = left + (right - left) / 2;
    mergeSortArrayRecursive(arr, left, mid);
    mergeSortArrayRecursive(arr, mid + 1, right);
    mergeArrays(arr, left, mid, right);
}

// Wrapper function to sort the custom TransactionArray by date
void sortTransactionsArrayByDate(TransactionArray& transactions) {
    if (transactions.size() <= 1) return;
    mergeSortArrayRecursive(transactions.getDataPtr(), 0, transactions.size() - 1);
}

// --- File Reading Implementations ---

// Read transactions into a Linked List
void readTransactionsFileLL(const std::string& filename, TransactionNode*& head) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl; // Use cerr for errors
        return;
    }
    std::cout << "Successfully opened " << filename << " for Linked List." << std::endl;

    std::string line;
    int lineNumber = 0;
    // Skip header
    if (!std::getline(file, line)) {
        std::cerr << "Error: File " << filename << " is empty or header is missing." << std::endl;
        return;
    }
    lineNumber++; // Account for header line

    while (std::getline(file, line)) {
        lineNumber++;
        TransactionNode* newTransaction = new TransactionNode();
        std::istringstream ss(line);
        std::string token;
        std::string customerPart, productPart; // For Customer|Product split

        try {
            // Handle Customer|Product format or standard CSV
            if (!std::getline(ss, token, ',')) { throw std::runtime_error("Missing Customer/Product field"); }
            size_t pipePos = token.find('|');
            if (pipePos != std::string::npos) {
                customerPart = token.substr(0, pipePos);
                productPart = token.substr(pipePos + 1);
                 // Check if customer part actually contains "CUST" if that's a strict rule
                size_t custPos = customerPart.find("CUST");
                if (custPos == std::string::npos) {
                     // If CUST is not found, assume standard format
                     productPart = customerPart; // The first part was the product
                     if (!std::getline(ss, customerPart, ',')) { throw std::runtime_error("Missing CustomerID after pipe format assumed wrong");}
                     newTransaction->product = productPart;
                     newTransaction->customerID = customerPart;
                } else {
                    newTransaction->customerID = customerPart;
                    newTransaction->product = productPart;
                }

            } else {
                // Assume standard format: CustomerID, Product
                 newTransaction->customerID = token; // First token is CustomerID
                 if (!std::getline(ss, newTransaction->product, ',')) { throw std::runtime_error("Missing Product field"); }
            }


            if (!std::getline(ss, newTransaction->category, ',')) { throw std::runtime_error("Missing Category field"); }
            if (!std::getline(ss, token, ',')) { throw std::runtime_error("Missing Price field"); }
            // Robust price conversion
            try {
                newTransaction->price = std::stod(token);
            } catch (const std::invalid_argument& ia) {
                throw std::runtime_error("Invalid argument for Price conversion");
            } catch (const std::out_of_range& oor) {
                throw std::runtime_error("Price value out of range");
            }

            if (!std::getline(ss, newTransaction->date, ',')) { throw std::runtime_error("Missing Date field"); }
            // Basic date format validation (e.g., check length or pattern if needed)
            if (newTransaction->date.length() != 10 || newTransaction->date[4] != '-' || newTransaction->date[7] != '-') {
                 // Handle potentially incorrect date format - log or skip?
                 // For now, we accept it but could add stricter validation.
            }

            if (!std::getline(ss, newTransaction->paymentMethod)) { // Read the rest of the line
                 if (ss.eof()) { // Check if it was just the end of the line
                    // Payment method might be optional or the last field, handle appropriately
                    // newTransaction->paymentMethod = "Unknown"; // Or leave empty
                 } else {
                     throw std::runtime_error("Missing Payment Method field or extra data");
                 }
            }
             // Trim leading/trailing whitespace from payment method if necessary
             newTransaction->paymentMethod.erase(0, newTransaction->paymentMethod.find_first_not_of(" \t\n\r\f\v"));
             newTransaction->paymentMethod.erase(newTransaction->paymentMethod.find_last_not_of(" \t\n\r\f\v") + 1);


            // Add to linked list (prepend)
            newTransaction->next = head;
            head = newTransaction;

        } catch (const std::runtime_error& e) {
            std::cerr << "Warning: Skipping line " << lineNumber << " in " << filename << ". Reason: " << e.what() << std::endl;
            delete newTransaction; // Clean up allocated memory
        } catch (...) {
            std::cerr << "Warning: Skipping line " << lineNumber << " in " << filename << ". Reason: Unknown error." << std::endl;
            delete newTransaction; // Clean up allocated memory
        }
    }
    std::cout << "Successfully finished reading transactions for Linked List." << std::endl;
}


// Read transactions into a Custom Array (TransactionArray)
void readTransactionsFileArray(const std::string& filename, TransactionArray& transactions) { // Changed signature
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    std::cout << "Successfully opened " << filename << " for Custom Array." << std::endl; // Updated message

    std::string line;
    int lineNumber = 0;
    // Skip header
    if (!std::getline(file, line)) {
         std::cerr << "Error: File " << filename << " is empty or header is missing." << std::endl;
        return;
    }
    lineNumber++;

    while (std::getline(file, line)) {
        lineNumber++;
        TransactionData newTransaction; // Uses TransactionData struct (no next pointer)
        std::istringstream ss(line);
        std::string token;
        std::string customerPart, productPart;

         try {
            // Handle Customer|Product format or standard CSV (same logic as LL version)
            if (!std::getline(ss, token, ',')) { throw std::runtime_error("Missing Customer/Product field"); }
            size_t pipePos = token.find('|');
             if (pipePos != std::string::npos) {
                customerPart = token.substr(0, pipePos);
                productPart = token.substr(pipePos + 1);
                 size_t custPos = customerPart.find("CUST");
                 if (custPos == std::string::npos) {
                     productPart = customerPart;
                     if (!std::getline(ss, customerPart, ',')) { throw std::runtime_error("Missing CustomerID after pipe format assumed wrong");}
                     newTransaction.product = productPart;
                     newTransaction.customerID = customerPart;
                 } else {
                    newTransaction.customerID = customerPart;
                    newTransaction.product = productPart;
                 }
            } else {
                 newTransaction.customerID = token;
                 if (!std::getline(ss, newTransaction.product, ',')) { throw std::runtime_error("Missing Product field"); }
            }

            if (!std::getline(ss, newTransaction.category, ',')) { throw std::runtime_error("Missing Category field"); }
            if (!std::getline(ss, token, ',')) { throw std::runtime_error("Missing Price field"); }
             try {
                newTransaction.price = std::stod(token);
            } catch (const std::invalid_argument& ia) {
                throw std::runtime_error("Invalid argument for Price conversion");
            } catch (const std::out_of_range& oor) {
                throw std::runtime_error("Price value out of range");
            }

            if (!std::getline(ss, newTransaction.date, ',')) { throw std::runtime_error("Missing Date field"); }
             // Optional: Add date format validation here too

            if (!std::getline(ss, newTransaction.paymentMethod)) {
                  if (ss.eof()) {
                    // Handle missing optional payment method
                  } else {
                     throw std::runtime_error("Missing Payment Method field or extra data");
                  }
            }
             // Trim whitespace
             newTransaction.paymentMethod.erase(0, newTransaction.paymentMethod.find_first_not_of(" \t\n\r\f\v"));
             newTransaction.paymentMethod.erase(newTransaction.paymentMethod.find_last_not_of(" \t\n\r\f\v") + 1);

            // Add to custom array
            transactions.push_back(newTransaction); // Use push_back of TransactionArray

        } catch (const std::runtime_error& e) {
            std::cerr << "Warning: Skipping line " << lineNumber << " in " << filename << ". Reason: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Warning: Skipping line " << lineNumber << " in " << filename << ". Reason: Unknown error." << std::endl;
        }
    }
    std::cout << "Successfully finished reading transactions for Custom Array." << std::endl; // Updated message
}

// Keep readReviewsFile as is, assuming Review struct is appropriate
void readReviewsFile(const std::string& filename, Review*& head) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << std::endl;
        return;
    }
    std::cout << "Successfully opened " << filename << std::endl;
    
    std::string line;
    // Skip header
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        Review* newReview = new Review();
        std::istringstream ss(line);
        std::string token;
        
        std::getline(ss, newReview->productID, ',');
        std::getline(ss, newReview->customerID, ',');
        std::getline(ss, token, ',');
        newReview->rating = std::stoi(token);
        std::getline(ss, newReview->reviewText);
        
        newReview->next = head;
        head = newReview;
    }
}

// Counting functions
int countTransactionsLL(TransactionNode* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

size_t countTransactionsArray(const TransactionArray& transactions) { // Changed return type and parameter
    return transactions.size();
}

int countReviews(Review* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

// Calculate percentage (Linked List version)
double calculateElectronicsCreditCardPercentageLL(TransactionNode* head) {
    int totalElectronics = 0;
    int electronicsCreditCard = 0;

    TransactionNode* current = head; // Iterate using a temporary pointer
    while (current) {
        if (current->category == "Electronics") {
            totalElectronics++;
            if (current->paymentMethod == "Credit Card") {
                electronicsCreditCard++;
            }
        }
        current = current->next;
    }

    return totalElectronics > 0 ? (double)electronicsCreditCard / totalElectronics * 100.0 : 0.0; // Ensure floating point division
}

// Calculate percentage (Custom Array version)
double calculateElectronicsCreditCardPercentageArray(const TransactionArray& transactions) { // Changed parameter
    int totalElectronics = 0;
    int electronicsCreditCard = 0;

    for (size_t i = 0; i < transactions.size(); ++i) { // Iterate using index
        const TransactionData& transaction = transactions[i]; // Access using operator[] or at()
        if (transaction.category == "Electronics") {
            totalElectronics++;
            if (transaction.paymentMethod == "Credit Card") {
                electronicsCreditCard++;
            }
        }
    }

    return totalElectronics > 0 ? (double)electronicsCreditCard / totalElectronics * 100.0 : 0.0;
}

/**
 * Checks if a character is a symbol (non-alphanumeric and non-space)
 * @param c The character to check
 * @return true if the character is a symbol, false otherwise
 */
bool isSymbol(char c) {
    return !isalnum(c) && !isspace(c);
}

/**
 * Normalizes a word by removing symbols and converting to lowercase
 * @param word The word to normalize
 * @return The normalized version of the word
 */
std::string normalizeWord(const std::string& word) {
    std::string normalized;
    for (char c : word) {
        if (!isSymbol(c)) {
            normalized += tolower(c);
        }
    }
    return normalized;
}

/**
 * Checks if two words are similar using a simple distance metric
 * @param word1 First word to compare
 * @param word2 Second word to compare
 * @param maxDistance Maximum allowed difference between words
 * @return true if words are similar, false otherwise
 */
bool areWordsSimilar(const std::string& word1, const std::string& word2, int maxDistance = 2) {
    if (abs((int)word1.length() - (int)word2.length()) > maxDistance) {
        return false;
    }
    
    int distance = 0;
    for (size_t i = 0; i < std::min(word1.length(), word2.length()); ++i) {
        if (word1[i] != word2[i]) {
            distance++;
            if (distance > maxDistance) return false;
        }
    }
    return true;
}

/**
 * Processes text to extract words and symbols, storing them in a linked list
 * @param text The text to process
 * @param wordList Reference to the head of the word list
 */
void processText(const std::string& text, WordNode*& wordList) {
    std::string currentWord;
    std::string currentSymbols;
    
    for (char c : text) {
        if (isSymbol(c)) {
            if (!currentWord.empty()) {
                // Store the word before the symbol
                std::string normalized = normalizeWord(currentWord);
                if (!normalized.empty()) {
                    WordNode* newNode = new WordNode();
                    newNode->baseWord = normalized;
                    newNode->variation = currentWord;
                    newNode->frequency = 1;
                    newNode->next = wordList;
                    wordList = newNode;
                }
                currentWord.clear();
            }
            currentSymbols += c;
        } else if (isspace(c)) {
            if (!currentWord.empty()) {
                // Store the word
                std::string normalized = normalizeWord(currentWord);
                if (!normalized.empty()) {
                    WordNode* newNode = new WordNode();
                    newNode->baseWord = normalized;
                    newNode->variation = currentWord;
                    newNode->frequency = 1;
                    newNode->next = wordList;
                    wordList = newNode;
                }
                currentWord.clear();
            }
            if (!currentSymbols.empty()) {
                // Store the symbols
                WordNode* newNode = new WordNode();
                newNode->baseWord = currentSymbols;
                newNode->variation = currentSymbols;
                newNode->frequency = 1;
                newNode->next = wordList;
                wordList = newNode;
                currentSymbols.clear();
            }
        } else {
            if (!currentSymbols.empty()) {
                // Store the symbols
                WordNode* newNode = new WordNode();
                newNode->baseWord = currentSymbols;
                newNode->variation = currentSymbols;
                newNode->frequency = 1;
                newNode->next = wordList;
                wordList = newNode;
                currentSymbols.clear();
            }
            currentWord += c;
        }
    }
    
    // Handle any remaining word or symbols
    if (!currentWord.empty()) {
        std::string normalized = normalizeWord(currentWord);
        if (!normalized.empty()) {
            WordNode* newNode = new WordNode();
            newNode->baseWord = normalized;
            newNode->variation = currentWord;
            newNode->frequency = 1;
            newNode->next = wordList;
            wordList = newNode;
        }
    }
    if (!currentSymbols.empty()) {
        WordNode* newNode = new WordNode();
        newNode->baseWord = currentSymbols;
        newNode->variation = currentSymbols;
        newNode->frequency = 1;
        newNode->next = wordList;
        wordList = newNode;
    }
}

/**
 * Merges similar words in the list by combining their frequencies
 * @param wordList Reference to the head of the word list
 */
void mergeSimilarWords(WordNode*& wordList) {
    WordNode* current = wordList;
    while (current != nullptr) {
        WordNode* runner = current->next;
        WordNode* prev = current;
        
        while (runner != nullptr) {
            if (areWordsSimilar(current->baseWord, runner->baseWord)) {
                // Merge the frequencies
                current->frequency += runner->frequency;
                
                // Remove the similar word
                prev->next = runner->next;
                WordNode* toDelete = runner;
                runner = runner->next;
                delete toDelete;
            } else {
                prev = runner;
                runner = runner->next;
            }
        }
        current = current->next;
    }
}

/**
 * Displays word frequencies in descending order
 * @param wordList The head of the word list
 * @param limit Maximum number of words to display
 */
void displayWordFrequencies(WordNode* wordList, int limit = 10) {
    // Sort the list by frequency
    for (WordNode* i = wordList; i != nullptr; i = i->next) {
        for (WordNode* j = i->next; j != nullptr; j = j->next) {
            if (i->frequency < j->frequency) {
                std::swap(i->baseWord, j->baseWord);
                std::swap(i->variation, j->variation);
                std::swap(i->frequency, j->frequency);
            }
        }
    }
    
    // Display top words
    int count = 0;
    WordNode* current = wordList;
    while (current != nullptr && count < limit) {
        std::cout << (count + 1) << ". " << current->variation 
                  << " (base: " << current->baseWord << "): " 
                  << current->frequency << " occurrences" << std::endl;
        current = current->next;
        count++;
    }
}

/**
 * Processes one-star reviews to extract and analyze words
 * @param head The head of the review list
 * @param wordFreq Reference to the head of the word frequency list
 */
void findOneStarReviewWords(Review* head, WordFrequency*& wordFreq) {
    WordNode* wordList = nullptr;
    
    while (head) {
        if (head->rating == 1) {
            processText(head->reviewText, wordList);
        }
        head = head->next;
    }
    
    mergeSimilarWords(wordList);
    
    // Convert WordNode list to WordFrequency list
    WordNode* current = wordList;
    while (current != nullptr) {
        WordFrequency* newWord = new WordFrequency();
        newWord->word = current->variation;
        newWord->frequency = current->frequency;
        newWord->next = wordFreq;
        wordFreq = newWord;
        
        WordNode* toDelete = current;
        current = current->next;
        delete toDelete;
    }
}

/**
 * Sorts words by frequency in descending order
 * @param head Reference to the head of the word frequency list
 */
void sortWordsByFrequency(WordFrequency*& head) {
    if (!head || !head->next) return;
    
    WordFrequency* sorted = nullptr;
    WordFrequency* current = head;
    
    while (current) {
        WordFrequency* next = current->next;
        
        if (!sorted || sorted->frequency <= current->frequency) {
            current->next = sorted;
            sorted = current;
        } else {
            WordFrequency* search = sorted;
            while (search->next && search->next->frequency > current->frequency) {
                search = search->next;
            }
            current->next = search->next;
            search->next = current;
        }
        current = next;
    }
    
    head = sorted;
}

/**
 * Displays the top words by frequency
 * @param head The head of the word frequency list
 * @param limit Maximum number of words to display
 */
void displayTopWords(WordFrequency* head, int limit) {
    int count = 0;
    WordFrequency* current = head;
    int prevFreq = -1;
    int rank = 0;
    
    while (current && count < limit) {
        if (current->frequency != prevFreq) {
            rank = count + 1;
        }
        std::cout << rank << ". " << current->word << ": " << current->frequency << " occurrences" << std::endl;
        prevFreq = current->frequency;
        current = current->next;
        count++;
    }
}

/**
 * Counts the number of words in a WordFrequency list
 * @param head The head of the word frequency list
 * @return The total count of words
 */
int countWordFrequency(WordFrequency* head) {
    int count = 0;
    WordFrequency* current = head; // Use a temporary pointer
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

// Function declaration for displayTopWords
void displayTopWords(WordFrequency* head, int limit);

// Function declaration for countWordFrequency
int countWordFrequency(WordFrequency* head);

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
    SortingAlgorithms::mergeSortLL(transactionsLL);
    auto endLLSort = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationLLSort = endLLSort - startLLSort;
    std::cout << "Linked List Merge Sort Time: " << durationLLSort.count() << " ms" << std::endl;

    // Time Custom Array Sort (Merge Sort by Date)
    auto startArrSort = std::chrono::high_resolution_clock::now();
    SortingAlgorithms::mergeSortArray(transactionsArray);
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
    // Note: Review analysis functions (processText, mergeSimilarWords, etc.) might need updates
    // if the WordNode/WordFrequency structures were intended for the array vs LL comparison too.
    // Currently, they are separate and use their own linked lists.
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

// Remove or comment out KeithHeapSort class usage if it's not defined here
// Assuming KeithHeapSort was defined in KeithHPP.hpp and is no longer needed for date sort.
/*
class KeithHeapSort {
    // ... implementation ...
public:
    void sortTransactions(TransactionNode*& head) {
        // ... implementation (likely sorts by price based on previous main) ...
        std::cout << "Warning: KeithHeapSort::sortTransactions called, but requirement is date sort." << std::endl;
    }
};
*/

// Note: Ensure Review struct and related functions (findOneStarReviewWords, etc.)
// are correctly defined and function as intended. The WordNode/WordFrequency parts
// were kept as linked lists for the review analysis task as specified.
// The comparison was focused on the Transaction data structure requirement.