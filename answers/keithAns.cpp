#include "../include/KeithHPP.hpp"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
//break down the sentence into words, put the words into linked list, and find any and all similar words instead of using this StopWord, need to break down the sentences into what expressions (need to detect symbols)

// Structure to store word variations and their frequencies
struct WordNode {
    std::string baseWord;
    std::string variation;
    int frequency;
    WordNode* next;
    
    WordNode() : frequency(0), next(nullptr) {}
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

// Implementation of merge sort for transactions
Transaction* merge(Transaction* left, Transaction* right) {
    if (!left) return right;
    if (!right) return left;
    
    Transaction* result = nullptr;
    
    if (left->date <= right->date) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }
    
    return result;
}

void split(Transaction* source, Transaction*& left, Transaction*& right) {
    if (!source || !source->next) {
        left = source;
        right = nullptr;
        return;
    }
    
    Transaction* slow = source;
    Transaction* fast = source->next;
    
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    
    left = source;
    right = slow->next;
    slow->next = nullptr;
}

void mergeSortTransactions(Transaction*& head) {
    if (!head || !head->next) return;
    
    Transaction* left;
    Transaction* right;
    
    split(head, left, right);
    
    mergeSortTransactions(left);
    mergeSortTransactions(right);
    
    head = merge(left, right);
}

// File reading implementations
void readTransactionsFile(const std::string& filename, Transaction*& head) {
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
        Transaction* newTransaction = new Transaction();
        std::istringstream ss(line);
        std::string token;
        
        // Handle Customer|Product format
        std::getline(ss, token, '|');
        size_t pos = token.find("CUST");
        if (pos != std::string::npos) {
            newTransaction->customerID = token.substr(pos);
            std::getline(ss, newTransaction->product, ',');
        } else {
            std::getline(ss, newTransaction->customerID, ',');
            newTransaction->product = token;
        }
        
        std::getline(ss, newTransaction->category, ',');
        std::getline(ss, token, ',');
        newTransaction->price = std::stod(token);
        std::getline(ss, newTransaction->date, ',');
        std::getline(ss, newTransaction->paymentMethod);
        
        newTransaction->next = head;
        head = newTransaction;
    }
    
    std::cout << "Successfully read transactions file." << std::endl;
}

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
int countTransactions(Transaction* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

int countReviews(Review* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

// Calculate percentage of electronics purchases with credit card
double calculateElectronicsCreditCardPercentage(Transaction* head) {
    int totalElectronics = 0;
    int electronicsCreditCard = 0;
    
    while (head) {
        if (head->category == "Electronics") {
            totalElectronics++;
            if (head->paymentMethod == "Credit Card") {
                electronicsCreditCard++;
            }
        }
        head = head->next;
    }
    
    return totalElectronics > 0 ? (double)electronicsCreditCard / totalElectronics * 100 : 0;
}

// Function to check if a character is a symbol
bool isSymbol(char c) {
    return !isalnum(c) && !isspace(c);
}

// Function to normalize a word (remove symbols, convert to lowercase)
std::string normalizeWord(const std::string& word) {
    std::string normalized;
    for (char c : word) {
        if (!isSymbol(c)) {
            normalized += tolower(c);
        }
    }
    return normalized;
}

// Function to check if two words are similar (using Levenshtein distance)
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

// Function to process text and extract words with symbols
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

// Function to merge similar words in the list
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

// Function to display word frequencies
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

void sortWordsByFrequency(WordFrequency*& head) {
    if (!head || !head->next) return;
    
    WordFrequency* sorted = nullptr;
    WordFrequency* current = head;
    int wordCount = 0;
    
    while (current) {
        wordCount++;
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
    std::cout << "Found " << wordCount << " unique meaningful words." << std::endl;
}

void displayTopWords(WordFrequency* head, int limit) {
    int count = 0;
    WordFrequency* current = head;
    
    // First pass: count total words and find minimum frequency
    int totalWords = 0;
    int minFreq = -1;
    while (current) {
        if (minFreq == -1 || current->frequency < minFreq) {
            minFreq = current->frequency;
        }
        totalWords++;
        current = current->next;
    }
    
    // Second pass: display words
    current = head;
    int prevFreq = -1;
    int rank = 0;
    count = 0;
    
    while (current && count < limit) {
        if (current->frequency != prevFreq) {
            rank = count + 1;
        }
        std::cout << rank << ". " << current->word << ": " << current->frequency << " occurrences" << std::endl;
        prevFreq = current->frequency;
        current = current->next;
        count++;
    }
    
    if (count == 0) {
        std::cout << "No words found." << std::endl;
    }
}

int main() {
    // Initialize data structures
    Transaction* transactions = nullptr;
    Review* reviews = nullptr;
    WordFrequency* wordFreq = nullptr;

    // Read data from files
    readTransactionsFile("data/transactionsClean.csv", transactions);
    readReviewsFile("data/reviewsClean.csv", reviews);

    // Question 1: Sort transactions by price using heap sort
    KeithHeapSort heapSorter;
    heapSorter.sortTransactions(transactions);
    
    // Display sorted transactions
    std::cout << "\nSorted Transactions (using Heap Sort):" << std::endl;
    Transaction* current = transactions;
    int displayCount = 0;
    while (current && displayCount < 10) {  // Show first 10 transactions
        std::cout << (displayCount + 1) << ". Product: " << current->product 
                  << ", Price: $" << std::fixed << std::setprecision(2) << current->price << std::endl;
        current = current->next;
        displayCount++;
    }
    if (current) {
        std::cout << "..." << std::endl;  // Indicate there are more transactions
    }

    // Question 2: Calculate percentage of electronics purchases made with credit card
    double percentage = calculateElectronicsCreditCardPercentage(transactions);
    std::cout << "\nPercentage of electronics purchases made with credit card: " 
              << std::fixed << std::setprecision(2) << percentage << "%" << std::endl;

    // Question 3: Find and display top words in 1-star reviews
    // Count one-star reviews first
    int oneStarCount = 0;
    Review* reviewCurrent = reviews;
    while (reviewCurrent) {
        if (reviewCurrent->rating == 1) oneStarCount++;
        reviewCurrent = reviewCurrent->next;
    }
    
    findOneStarReviewWords(reviews, wordFreq);
    sortWordsByFrequency(wordFreq);
    
    std::cout << "\nAnalyzed " << oneStarCount << " one-star reviews out of " 
              << countReviews(reviews) << " total reviews." << std::endl;
    std::cout << "Found " << countWordFrequency(wordFreq) << " unique meaningful words in 1-star reviews." << std::endl;
    std::cout << "\nTop 10 most frequent words in 1-star reviews:" << std::endl;
    displayTopWords(wordFreq, 10);

    // Clean up memory
    while (transactions) {
        Transaction* temp = transactions;
        transactions = transactions->next;
        delete temp;
    }

    while (reviews) {
        Review* temp = reviews;
        reviews = reviews->next;
        delete temp;
    }

    while (wordFreq) {
        WordFrequency* temp = wordFreq;
        wordFreq = wordFreq->next;
        delete temp;
    }

    // Add pause before closing
    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}

// Helper function to count words in WordFrequency list
int countWordFrequency(WordFrequency* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
} 