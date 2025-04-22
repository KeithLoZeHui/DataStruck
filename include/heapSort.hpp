#ifndef HEAPSORT_HPP
#define HEAPSORT_HPP

#include "linkedList.hpp"
#include <string>
#include <iostream>
#include <stdexcept>

// Forward declarations
struct TransactionNode;

// Heap Node structure
struct HeapNode {
    double data;
    HeapNode* parent;
    HeapNode* left;
    HeapNode* right;
    HeapNode* next;

    HeapNode(double val) : data(val), parent(nullptr), left(nullptr), right(nullptr), next(nullptr) {}
};

// Heap Linked List class
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

// Heap Sort class
class HeapSort {
public:
    static void sortTransactions(TransactionNode*& head);
    static void processTransactions(const std::string& filename);
};

#endif // HEAPSORT_HPP 