#include "../../include/KeithHPP.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

struct Review {
    string productID, customerID, reviewText;
    int rating;
};

// Node structures for linked lists
struct StringNode {
    string data;
    StringNode* next;
    StringNode(const string& str) : data(str), next(nullptr) {}
};

struct ReviewNode {
    Review data;
    ReviewNode* next;
    ReviewNode(const Review& rev) : data(rev), next(nullptr) {}
};

struct WordFreqNode {
    string word;
    int frequency;
    WordFreqNode* next;
    WordFreqNode(const string& w, int f) : word(w), frequency(f), next(nullptr) {}
};

// Linked List implementations
class StringList {
private:
    StringNode* head;
    StringNode* tail;
    int size;

public:
    StringList() : head(nullptr), tail(nullptr), size(0) {}
    
    ~StringList() {
        while (head) {
            StringNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void add(const string& str) {
        StringNode* newNode = new StringNode(str);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }
    
    StringNode* getHead() const { return head; }
    int getSize() const { return size; }
};

class ReviewList {
private:
    ReviewNode* head;
    ReviewNode* tail;
    int size;

public:
    ReviewList() : head(nullptr), tail(nullptr), size(0) {}
    
    ~ReviewList() {
        while (head) {
            ReviewNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void add(const Review& review) {
        ReviewNode* newNode = new ReviewNode(review);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }
    
    ReviewNode* getHead() const { return head; }
    bool empty() const { return head == nullptr; }
    int getSize() const { return size; }
};

class WordFreqList {
private:
    WordFreqNode* head;
    int size;

public:
    WordFreqList() : head(nullptr), size(0) {}
    
    ~WordFreqList() {
        while (head) {
            WordFreqNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void add(const string& word, int freq) {
        WordFreqNode* newNode = new WordFreqNode(word, freq);
        
        // Insert in sorted order (by frequency, descending)
        if (!head || freq > head->frequency) {
            newNode->next = head;
            head = newNode;
        } else {
            WordFreqNode* current = head;
            while (current->next && current->next->frequency >= freq) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        size++;
    }
    
    WordFreqNode* getHead() const { return head; }
    int getSize() const { return size; }
    
    WordFreqList* getTopN(int n) {
        WordFreqList* result = new WordFreqList();
        WordFreqNode* current = head;
        int count = 0;
        
        while (current && count < n) {
            result->add(current->word, current->frequency);
            current = current->next;
            count++;
        }
        
        return result;
    }
};

class ReviewAnalyzer {
private:
    WordFreqList wordFrequency;

    StringList tokenizeText(const string& text) {
        StringList tokens;
        string word;
        stringstream ss(text);
        
        while (ss >> word) {
            string cleanWord;
            for (char c : word) {
                if (isalnum(c)) {
                    cleanWord += tolower(c);
                }
            }
            
            if (cleanWord.length() > 2) {
                tokens.add(cleanWord);
            }
        }
        return tokens;
    }

    void processReview(const Review& review) {
        if (review.rating == 1) {
            StringList tokens = tokenizeText(review.reviewText);
            for (StringNode* curr = tokens.getHead(); curr != nullptr; curr = curr->next) {
                bool found = false;
                WordFreqNode* current = wordFrequency.getHead();
                while (current) {
                    if (current->word == curr->data) {
                        current->frequency++;
                        found = true;
                        break;
                    }
                    current = current->next;
                }
                if (!found) {
                    wordFrequency.add(curr->data, 1);
                }
            }
        }
    }

public:
    void analyzeReviews(const ReviewList& reviews) {
        for (ReviewNode* curr = reviews.getHead(); curr != nullptr; curr = curr->next) {
            processReview(curr->data);
        }
    }

    WordFreqList* getTopWords(int n) {
        return wordFrequency.getTopN(n);
    }

    void printResults(int topN = 10) {
        cout << "\nTop " << topN << " most frequent words in 1-star reviews:\n";
        cout << "----------------------------------------\n";
        
        WordFreqList* topWords = getTopWords(topN);
        WordFreqNode* current = topWords->getHead();
        
        while (current) {
            cout << current->word << ": " << current->frequency << " occurrences\n";
            current = current->next;
        }
        
        delete topWords;
    }
};

ReviewList loadReviews(const string& filename) {
    ReviewList reviews;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Error: Cannot open " << filename << endl;
        return reviews;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        Review review;
        string rating;

        getline(ss, review.productID, ',');
        getline(ss, review.customerID, ',');
        getline(ss, rating, ',');
        getline(ss, review.reviewText);

        try {
            review.rating = stoi(rating);
            if (review.rating >= 1 && review.rating <= 5) {
                reviews.add(review);
            }
        } catch (...) {
            continue;
        }
    }

    return reviews;
}

int main() {
    ReviewList reviews = loadReviews("data/reviewsClean.csv");
    
    if (reviews.empty()) {
        cout << "No reviews loaded. Exiting...\n";
        return 1;
    }
    
    ReviewAnalyzer analyzer;
    analyzer.analyzeReviews(reviews);
    analyzer.printResults(10);
    
    return 0;
}
