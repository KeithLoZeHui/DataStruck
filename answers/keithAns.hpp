#ifndef KEITH_ANS_HPP
#define KEITH_ANS_HPP

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "../src/algorithms/KeithJumpSearch.cpp"
#include "../src/algorithms/heapSort.cpp"
#include <cctype>     
#include <chrono>
#include <stdexcept>
#include <limits>
#include <fstream>
#include <vector>

// Structure to store word variations and their frequencies
struct WordNode {
    MyString baseWord;
    MyString variation;
    int frequency;
    WordNode* next;
    
    WordNode() : frequency(0), next(nullptr) {}
};

// Helper function to split string into words
inline void splitIntoWords(const char* text, WordFrequency*& wordFreq) {
    std::string stdText(text);
    std::string stdWord;
    std::istringstream iss(stdText);
    
    while (iss >> stdWord) {
        // Convert to lowercase and remove punctuation
        std::string cleanStdWord;
        for (char c : stdWord) {
            if (isalpha(c)) {
                cleanStdWord += tolower(c);
            }
        }
        
        if (cleanStdWord.empty()) continue;
        
        // Convert to MyString for comparison
        MyString cleanWord(cleanStdWord.c_str());
        
        // Update word frequency
        WordFrequency* current = wordFreq;
        bool found = false;
        
        while (current) {
            if (current->word == cleanWord) {
                current->frequency++;
                found = true;
                break;
            }
            current = current->next;
        }
        
        if (!found) {
            // Add new word to the list
            WordFrequency* newWord = new WordFrequency;
            newWord->word = cleanWord;
            newWord->frequency = 1;
            newWord->next = wordFreq;
            wordFreq = newWord;
        }
    }
}

// Implementation of merge sort for transactions (Linked List by Date)
inline TransactionNode* merge(TransactionNode* left, TransactionNode* right) {
    TransactionNode* result = nullptr;
    
    // Base cases
    if (!left) return right;
    if (!right) return left;
    
    // Compare dates and merge
    if (left->date <= right->date) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }
    
    return result;
}

inline void split(TransactionNode* source, TransactionNode*& left, TransactionNode*& right) {
    if (!source || !source->next) {
        left = source;
        right = nullptr;
        return;
    }
    
    // Use slow and fast pointers to find the middle
    TransactionNode* slow = source;
    TransactionNode* fast = source->next;
    
    // Fast moves twice as fast as slow
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    
    // Split the list into two halves
    left = source;
    right = slow->next;
    slow->next = nullptr; // Terminate first half
}



// Merge function for TransactionData arrays (sorted by date)
inline void mergeArrays(TransactionData* arr, size_t left, size_t mid, size_t right) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;
    
    // Create temporary arrays
    TransactionData* L = new TransactionData[n1];
    TransactionData* R = new TransactionData[n2];
    
    // Copy data to temporary arrays L[] and R[]
    for (size_t i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (size_t j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    // Merge the temporary arrays back into arr[left..right]
    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].date <= R[j].date) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    // Free temporary arrays
    delete[] L;
    delete[] R;
}

// Main Merge Sort function for TransactionData arrays (recursive)
inline void mergeSortArrayRecursive(TransactionData* arr, size_t left, size_t right) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        
        // Sort first and second halves
        mergeSortArrayRecursive(arr, left, mid);
        mergeSortArrayRecursive(arr, mid + 1, right);
        
        // Merge the sorted halves
        mergeArrays(arr, left, mid, right);
    }
}

// Wrapper function to sort the custom TransactionArray by date
inline void sortTransactionsArrayByDate(TransactionArray& transactions) {
    if (transactions.size() > 1) {
        mergeSortArrayRecursive(transactions.getDataPtr(), 0, transactions.size() - 1);
    }
}

// Read transactions into a Linked List
inline void readTransactionsFileLL(const std::string& filename, TransactionNode*& head) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    std::string line;
    // Skip header line
    std::getline(file, line);
    
    TransactionNode* tail = nullptr; // For appending to the end
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        // Split line by comma
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }
        
        // Ensure we have all required fields
        if (tokens.size() >= 6) {
            TransactionNode* newNode = new TransactionNode;
            
            // Parse and store transaction data
            newNode->customerID = tokens[0].c_str();
            newNode->product = tokens[1].c_str();
            
            // Convert price from string to double
            try {
                newNode->price = std::stod(tokens[2]);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid price format: " << tokens[2] << std::endl;
                newNode->price = 0.0; // Default value
            }
            
            newNode->date = tokens[3].c_str();
            newNode->category = tokens[4].c_str();
            newNode->paymentMethod = tokens[5].c_str();
            
            newNode->next = nullptr;
            
            // Add to linked list
            if (!head) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        } else {
            std::cerr << "Invalid line format: " << line << std::endl;
        }
    }
    
    file.close();
}

// Read transactions into a Custom Array (TransactionArray)
inline void readTransactionsFileArray(const std::string& filename, TransactionArray& transactions) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    std::string line;
    // Skip header line
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        // Split line by comma
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }
        
        // Ensure we have all required fields
        if (tokens.size() >= 6) {
            TransactionData transaction;
            
            // Parse and store transaction data
            transaction.customerID = tokens[0].c_str();
            transaction.product = tokens[1].c_str();
            
            // Convert price from string to double
            try {
                transaction.price = std::stod(tokens[2]);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid price format: " << tokens[2] << std::endl;
                transaction.price = 0.0; // Default value
            }
            
            transaction.date = tokens[3].c_str();
            transaction.category = tokens[4].c_str();
            transaction.paymentMethod = tokens[5].c_str();
            
            // Add to custom array
            transactions.push_back(transaction);
        } else {
            std::cerr << "Invalid line format: " << line << std::endl;
        }
    }
    
    file.close();
}

// Keep readReviewsFile as is, assuming Review struct is appropriate
inline void readReviewsFile(const std::string& filename, Review*& head) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    std::string line;
    // Skip header line
    std::getline(file, line);
    
    Review* tail = nullptr; // For appending to the end
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        // Split line by comma, but preserve commas in the review text
        // This assumes review text is the last field and may contain commas
        for (int i = 0; i < 3 && std::getline(iss, token, ','); i++) {
            tokens.push_back(token);
        }
        
        // Get the rest as review text (may contain commas)
        std::string reviewText;
        if (std::getline(iss, reviewText)) {
            tokens.push_back(reviewText);
        }
        
        // Ensure we have all required fields
        if (tokens.size() >= 4) {
            Review* newReview = new Review;
            
            newReview->productID = tokens[0].c_str();
            newReview->customerID = tokens[1].c_str();
            
            // Convert rating from string to int
            try {
                newReview->rating = std::stoi(tokens[2]);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid rating format: " << tokens[2] << std::endl;
                newReview->rating = 0; // Default value
            }
            
            newReview->reviewText = tokens[3].c_str();
            newReview->next = nullptr;
            
            // Add to linked list
            if (!head) {
                head = newReview;
                tail = newReview;
            } else {
                tail->next = newReview;
                tail = newReview;
            }
        } else {
            std::cerr << "Invalid review line format: " << line << std::endl;
        }
    }
    
    file.close();
}

// Counting functions
inline int countTransactionsLL(TransactionNode* head) {
    int count = 0;
    TransactionNode* current = head;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

inline size_t countTransactionsArray(const TransactionArray& transactions) {
    return transactions.size();
}

inline int countReviews(Review* head) {
    int count = 0;
    Review* current = head;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

// Calculate percentage (Linked List version)
inline double calculateElectronicsCreditCardPercentageLL(TransactionNode* head) {
    int electronicsTotal = 0;
    int electronicsCreditCard = 0;
    
    TransactionNode* current = head;
    while (current) {
        if (current->category == "Electronics") {
            electronicsTotal++;
            if (current->paymentMethod == "Credit Card") {
                electronicsCreditCard++;
            }
        }
        current = current->next;
    }
    
    if (electronicsTotal == 0) return 0.0;
    return (electronicsCreditCard * 100.0) / electronicsTotal;
}

// Calculate percentage (Custom Array version)
inline double calculateElectronicsCreditCardPercentageArray(const TransactionArray& transactions) {
    int electronicsTotal = 0;
    int electronicsCreditCard = 0;
    
    for (size_t i = 0; i < transactions.size(); i++) {
        if (transactions[i].category == "Electronics") {
            electronicsTotal++;
            if (transactions[i].paymentMethod == "Credit Card") {
                electronicsCreditCard++;
            }
        }
    }
    
    if (electronicsTotal == 0) return 0.0;
    return (electronicsCreditCard * 100.0) / electronicsTotal;
}

// Checks if a character is a symbol (non-alphanumeric and non-space)
inline bool isSymbol(char c) {
    return !isalnum(c) && !isspace(c);
}

// Normalizes a word by removing symbols and converting to lowercase
inline MyString normalizeWord(const MyString& word) {
    std::string stdWord = word.c_str();
    std::string normalized;
    
    for (char c : stdWord) {
        if (isalpha(c)) {
            normalized += tolower(c);
        }
    }
    
    return MyString(normalized.c_str());
}

// Checks if two words are similar using a simple distance metric
inline bool areWordsSimilar(const MyString& word1, const MyString& word2, int maxDistance = 2) {
    std::string s1 = word1.c_str();
    std::string s2 = word2.c_str();
    
    // If length difference is too great, words are not similar
    if (abs((int)s1.length() - (int)s2.length()) > maxDistance) {
        return false;
    }
    
    // Simple character-by-character comparison
    int differences = 0;
    size_t minLength = std::min(s1.length(), s2.length());
    
    for (size_t i = 0; i < minLength; i++) {
        if (s1[i] != s2[i]) differences++;
        if (differences > maxDistance) return false;
    }
    
    // Add remaining length difference to differences
    differences += abs((int)s1.length() - (int)s2.length());
    
    return differences <= maxDistance;
}

// Process text to extract words and symbols
inline void processText(const char* text, WordFrequency*& wordFreq) {
    std::string stdText = text;
    std::string currentWord;
    
    for (size_t i = 0; i < stdText.length(); i++) {
        char c = stdText[i];
        
        if (isalpha(c)) {
            // Part of a word
            currentWord += c;
        } else {
            // End of word or symbol
            if (!currentWord.empty()) {
                // Process completed word
                MyString word(currentWord.c_str());
                MyString normalizedWord = normalizeWord(word);
                
                // Skip very short words (likely not meaningful)
                if (std::string(normalizedWord.c_str()).length() > 1) {
                    // Update word frequency
                    WordFrequency* current = wordFreq;
                    bool found = false;
                    
                    while (current) {
                        if (current->word == normalizedWord) {
                            current->frequency++;
                            found = true;
                            break;
                        }
                        current = current->next;
                    }
                    
                    if (!found) {
                        // Add new word to the list
                        WordFrequency* newWord = new WordFrequency;
                        newWord->word = normalizedWord;
                        newWord->frequency = 1;
                        newWord->next = wordFreq;
                        wordFreq = newWord;
                    }
                }
                
                currentWord.clear();
            }
            
            // Process symbol if it's not whitespace
            if (isSymbol(c)) {
                std::string symbolStr(1, c);
                MyString symbol(symbolStr.c_str());
                
                // Update symbol frequency
                WordFrequency* current = wordFreq;
                bool found = false;
                
                while (current) {
                    if (current->word == symbol) {
                        current->frequency++;
                        found = true;
                        break;
                    }
                    current = current->next;
                }
                
                if (!found) {
                    // Add new symbol to the list
                    WordFrequency* newSymbol = new WordFrequency;
                    newSymbol->word = symbol;
                    newSymbol->frequency = 1;
                    newSymbol->next = wordFreq;
                    wordFreq = newSymbol;
                }
            }
        }
    }
    
    // Process any remaining word
    if (!currentWord.empty()) {
        MyString word(currentWord.c_str());
        MyString normalizedWord = normalizeWord(word);
        
        if (std::string(normalizedWord.c_str()).length() > 1) {
            // Update word frequency
            WordFrequency* current = wordFreq;
            bool found = false;
            
            while (current) {
                if (current->word == normalizedWord) {
                    current->frequency++;
                    found = true;
                    break;
                }
                current = current->next;
            }
            
            if (!found) {
                // Add new word to the list
                WordFrequency* newWord = new WordFrequency;
                newWord->word = normalizedWord;
                newWord->frequency = 1;
                newWord->next = wordFreq;
                wordFreq = newWord;
            }
        }
    }
}

// Merge similar words in the list
inline void mergeSimilarWords(WordFrequency*& wordFreq) {
    WordFrequency* current = wordFreq;
    
    while (current) {
        WordFrequency* runner = current->next;
        WordFrequency* prev = current;
        
        while (runner) {
            if (areWordsSimilar(current->word, runner->word, 2)) {
                // Merge similar words (keep the one with higher frequency)
                if (current->frequency >= runner->frequency) {
                    current->frequency += runner->frequency;
                    // Remove runner from the list
                    prev->next = runner->next;
                    WordFrequency* temp = runner;
                    runner = runner->next;
                    delete temp;
                } else {
                    runner->frequency += current->frequency;
                    // Keep runner's word as the base word
                    // We'll need to update all references to current later
                    // For now, just continue
                    prev = runner;
                    runner = runner->next;
                }
            } else {
                prev = runner;
                runner = runner->next;
            }
        }
        
        current = current->next;
    }
}

// Display word frequencies in descending order
inline void displayWordFrequencies(WordFrequency* wordFreq, int limit) {
    // Convert linked list to vector for sorting
    std::vector<std::pair<MyString, int>> words;
    WordFrequency* current = wordFreq;
    
    while (current) {
        words.push_back({current->word, current->frequency});
        current = current->next;
    }
    
    // Sort by frequency (descending)
    // std::sort removed; use HeapSort or KeithJumpSearch instead(words.begin(), words.end(), 
              [](const std::pair<MyString, int>& a, const std::pair<MyString, int>& b) {
        return a.second > b.second;
    });
    
    // Display results
    std::cout << "Word Frequencies:" << std::endl;
    std::cout << "---------------" << std::endl;
    
    int count = 0;
    for (const auto& pair : words) {
        std::cout << pair.first.c_str() << ": " << pair.second << std::endl;
        count++;
        if (limit > 0 && count >= limit) break;
    }
}

// Find words in one-star reviews
inline void findOneStarReviewWords(Review* reviews, WordFrequency*& wordFreq) {
    Review* current = reviews;
    
    while (current) {
        if (current->rating == 1) {
            // Process the review text
            processText(current->reviewText.c_str(), wordFreq);
        }
        current = current->next;
    }
    
    // Merge similar words to get more accurate frequencies
    mergeSimilarWords(wordFreq);
}

// Sort words by frequency
inline void sortWordsByFrequency(WordFrequency*& wordFreq) {
    // If list is empty or has only one node, it's already sorted
    if (!wordFreq || !wordFreq->next) return;
    
    // Convert to vector for sorting
    std::vector<WordFrequency*> words;
    WordFrequency* current = wordFreq;
    
    while (current) {
        words.push_back(current);
        current = current->next;
    }
    
    // Sort by frequency (descending)
    // std::sort removed; use HeapSort or KeithJumpSearch instead(words.begin(), words.end(), 
              [](const WordFrequency* a, const WordFrequency* b) {
        return a->frequency > b->frequency;
    });
    
    // Rebuild the linked list
    wordFreq = words[0];
    for (size_t i = 0; i < words.size() - 1; i++) {
        words[i]->next = words[i + 1];
    }
    words.back()->next = nullptr;
}

// Display top words by frequency
inline void displayTopWords(WordFrequency* wordFreq, int limit) {
    WordFrequency* current = wordFreq;
    int count = 0;
    
    while (current && count < limit) {
        std::cout << count + 1 << ". " << current->word.c_str()
                  << " (" << current->frequency << " occurrences)" << std::endl;
        current = current->next;
        count++;
    }
}

// Count words in frequency list
inline int countWordFrequency(WordFrequency* wordFreq) {
    int count = 0;
    WordFrequency* current = wordFreq;
    
    while (current) {
        count++;
        current = current->next;
    }
    
    return count;
}

#endif // KEITH_ANS_HPP
