#ifndef KEITHHPP_HPP
#define KEITHHPP_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Custom data structures
struct Transaction {
    std::string customerID;
    std::string product;
    double price;
    std::string date;
    std::string category;
    std::string paymentMethod;
    Transaction* next;
    
    Transaction() : next(nullptr) {}
};

struct Review {
    std::string productID;
    std::string customerID;
    int rating;
    std::string reviewText;
    Review* next;
    
    Review() : next(nullptr) {}
};

struct WordFrequency {
    std::string word;
    int frequency;
    WordFrequency* next;
    
    WordFrequency() : frequency(0), next(nullptr) {}
};

// Heap Sort related structures and classes
struct HeapNode {
    double data;
    HeapNode* next;
    HeapNode* left;
    HeapNode* right;
    HeapNode* parent;
    HeapNode(double value) : data(value), next(nullptr), left(nullptr), right(nullptr), parent(nullptr) {}
};

class HeapLinkedList {
private:
    HeapNode* root;
    HeapNode* last;
    int size;
    void heapifyUp(HeapNode* node);
    void heapifyDown(HeapNode* node);
    void swapValues(HeapNode* a, HeapNode* b);
    HeapNode* getParent(HeapNode* node);
    HeapNode* getLastParent();

public:
    HeapLinkedList();
    ~HeapLinkedList();
    void insert(double value);
    double extractMax();
    HeapNode* getRoot() const;
    int getSize() const;
    bool isEmpty() const;
};

class KeithHeapSort {
private:
    HeapLinkedList heap;
public:
    void sortTransactions(Transaction*& head);
    void processTransactions(const std::string& filename);
};

// Function declarations
void readTransactionsFile(const std::string& filename, Transaction*& head);
void readReviewsFile(const std::string& filename, Review*& head);
void mergeSortTransactions(Transaction*& head);
int countTransactions(Transaction* head);
int countReviews(Review* head);
int countWordFrequency(WordFrequency* head);
double calculateElectronicsCreditCardPercentage(Transaction* head);
void findOneStarReviewWords(Review* head, WordFrequency*& wordFreq);
void sortWordsByFrequency(WordFrequency*& head);
void displayTopWords(WordFrequency* head, int limit);

#endif
