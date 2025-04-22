#include "../../include/heapSort.hpp"
#include "../../include/linkedList.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>

// HeapLinkedList implementation
HeapLinkedList::HeapLinkedList() : root(nullptr), last(nullptr), size(0) {}

HeapLinkedList::~HeapLinkedList() {
    // Delete nodes level by level
    while (root) {
        HeapNode* current = root;
        root = nullptr;
        
        // Delete current level
        while (current) {
            HeapNode* next = current->next;
            delete current;
            current = next;
        }
    }
    last = nullptr;
    size = 0;
}

void HeapLinkedList::heapifyUp(HeapNode* node) {
    while (node && node->parent && node->data > node->parent->data) {
        swapValues(node, node->parent);
        node = node->parent;
    }
}

void HeapLinkedList::heapifyDown(HeapNode* node) {
    while (true) {
        HeapNode* largest = node;
        HeapNode* left = node->left;
        HeapNode* right = node->right;

        if (left && left->data > largest->data)
            largest = left;
        if (right && right->data > largest->data)
            largest = right;

        if (largest != node) {
            swapValues(node, largest);
            node = largest;
        } else {
            break;
        }
    }
}

void HeapLinkedList::swapValues(HeapNode* a, HeapNode* b) {
    double temp = a->data;
    a->data = b->data;
    b->data = temp;
}

HeapNode* HeapLinkedList::getParent(HeapNode* node) {
    return node->parent;
}

HeapNode* HeapLinkedList::getLastParent() {
    if (!last) return nullptr;
    return last->parent;
}

void HeapLinkedList::insert(double value) {
    HeapNode* newNode = new HeapNode(value);
    size++;

    if (!root) {
        root = last = newNode;
        return;
    }

    // Find position for new node using integer division
    int lastPos = size - 1;
    HeapNode* current = root;
    int height = 0;
    int temp = lastPos;
    while (temp > 0) {
        temp /= 2;
        height++;
    }

    // Navigate to parent of new node
    for (int i = 0; i < height && current; i++) {
        int bitPosition = height - 1 - i;
        bool goRight = (lastPos >> bitPosition) & 1;
        
        if (goRight && current->right)
            current = current->right;
        else if (!goRight && current->left)
            current = current->left;
        else
            break;
    }

    // Add new node
    if (!current->left)
        current->left = newNode;
    else
        current->right = newNode;

    newNode->parent = current;
    last = newNode;

    heapifyUp(newNode);
}

double HeapLinkedList::extractMax() {
    if (!root) throw std::runtime_error("Heap is empty");

    double maxVal = root->data;
    
    if (size == 1) {
        delete root;
        root = last = nullptr;
        size = 0;
        return maxVal;
    }

    // Replace root with last node
    root->data = last->data;

    // Remove last node
    HeapNode* oldLast = last;
    HeapNode* parent = last->parent;
    if (parent) {
        if (parent->right == last)
            parent->right = nullptr;
        else
            parent->left = nullptr;
    }

    // Update last pointer using integer division
    if (size > 2) {
        int newLastPos = size - 2;
        HeapNode* current = root;
        
        int height = 0;
        int temp = newLastPos;
        while (temp > 0) {
            temp /= 2;
            height++;
        }

        // Navigate to new last node
        for (int i = 0; i < height && current; i++) {
            int bitPosition = height - 1 - i;
            bool goRight = (newLastPos >> bitPosition) & 1;
            
            if (goRight && current->right)
                current = current->right;
            else if (!goRight && current->left)
                current = current->left;
            else
                break;
        }
        last = current;
    } else {
        last = root;
    }

    delete oldLast;
    size--;

    // Maintain heap property
    heapifyDown(root);

    return maxVal;
}

HeapNode* HeapLinkedList::getRoot() const { return root; }
int HeapLinkedList::getSize() const { return size; }
bool HeapLinkedList::isEmpty() const { return size == 0; }

// HeapSort implementation
void HeapSort::sortTransactions(TransactionNode*& head) {
    if (!head || !head->next) return;  // Nothing to sort

    // Count transactions
    int count = 0;
    TransactionNode* current = head;
    while (current) {
        count++;
        current = current->next;
    }

    // Create array of pointers to transactions
    TransactionNode** arr = new TransactionNode*[count];
    current = head;
    for (int i = 0; i < count; i++) {
        arr[i] = current;
        current = current->next;
    }

    // Heapify all non-leaf nodes
    for (int i = count / 2 - 1; i >= 0; i--) {
        // Heapify down
        int largest = i;
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < count && arr[left]->price > arr[largest]->price)
                largest = left;
            if (right < count && arr[right]->price > arr[largest]->price)
                largest = right;

            if (largest != i) {
                // Swap transactions
                TransactionNode* temp = arr[i];
                arr[i] = arr[largest];
                arr[largest] = temp;
                i = largest;
            } else {
                break;
            }
        }
    }

    // Extract max elements and rebuild heap
    TransactionNode* sorted = nullptr;
    for (int i = count - 1; i >= 0; i--) {
        // Add max to sorted list
        arr[0]->next = sorted;
        sorted = arr[0];

        // Move last element to root and heapify
        arr[0] = arr[i];
        
        int parent = 0;
        while (true) {
            int largest = parent;
            int left = 2 * parent + 1;
            int right = 2 * parent + 2;

            if (left < i && arr[left]->price > arr[largest]->price)
                largest = left;
            if (right < i && arr[right]->price > arr[largest]->price)
                largest = right;

            if (largest != parent) {
                TransactionNode* temp = arr[parent];
                arr[parent] = arr[largest];
                arr[largest] = temp;
                parent = largest;
            } else {
                break;
            }
        }
    }

    // Update head and clean up
    head = sorted;
    delete[] arr;
}

void HeapSort::processTransactions(const std::string& filename) {
    TransactionNode* transactions = nullptr;
    // Note: You'll need to implement readTransactionsFile or use an existing one
    // readTransactionsFile(filename, transactions);
    sortTransactions(transactions);
    
    // Display sorted transactions
    std::cout << "\nSorted Transactions (using Heap Sort):" << std::endl;
    TransactionNode* current = transactions;
    while (current) {
        std::cout << "Product: " << current->product 
             << ", Price: $" << std::fixed << std::setprecision(2) << current->price << std::endl;
        current = current->next;
    }

    // Clean up
    while (transactions) {
        TransactionNode* temp = transactions;
        transactions = transactions->next;
        delete temp;
    }
} 