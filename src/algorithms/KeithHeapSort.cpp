#include "../../include/KeithHPP.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

struct Node {
    double data;
    Node* next;
    Node* left;
    Node* right;
    Node* parent;
    Node(double value) : data(value), next(nullptr), left(nullptr), right(nullptr), parent(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}
    
    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    
    void append(double value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }
    
    Node* getHead() const { return head; }
    int getSize() const { return size; }
};

class HeapLinkedList {
private:
    Node* root;
    Node* last;
    int size;

    // Helper function to get parent index
    Node* getParent(Node* node) {
        return node->parent;
    }

    // Helper function to get last node's parent
    Node* getLastParent() {
        if (!last) return nullptr;
        return last->parent;
    }

    // Helper function to swap node values
    void swapValues(Node* a, Node* b) {
        double temp = a->data;
        a->data = b->data;
        b->data = temp;
    }

    // Heapify function for linked list based heap
    void heapifyUp(Node* node) {
        while (node && node->parent && node->data > node->parent->data) {
            swapValues(node, node->parent);
            node = node->parent;
        }
    }

    void heapifyDown(Node* node) {
        while (true) {
            Node* largest = node;
            Node* left = node->left;
            Node* right = node->right;

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

public:
    HeapLinkedList() : root(nullptr), last(nullptr), size(0) {}

    ~HeapLinkedList() {
        while (root) {
            Node* temp = root;
            root = root->next;
            delete temp;
        }
    }

    void insert(double value) {
        Node* newNode = new Node(value);
        size++;

        if (!root) {
            root = last = newNode;
            return;
        }

        // Find position for new node using integer division
        int lastPos = size - 1;
        Node* current = root;
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

    double extractMax() {
        if (!root) throw runtime_error("Heap is empty");

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
        Node* oldLast = last;
        if (last->parent->right == last)
            last->parent->right = nullptr;
        else
            last->parent->left = nullptr;

        // Update last pointer using integer division
        if (size > 2) {
            int newLastPos = size - 2;
            Node* current = root;
            
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

    Node* getRoot() const { return root; }
    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }
};

class KeithHeapSort {
private:
    HeapLinkedList heap;

public:
    // Sort using heap-based priority queue
    LinkedList* sort(LinkedList* list) {
        // Insert all elements into heap
        Node* current = list->getHead();
        while (current) {
            heap.insert(current->data);
            current = current->next;
        }

        // Extract elements in sorted order (descending)
        LinkedList* sortedList = new LinkedList();
        while (!heap.isEmpty()) {
            sortedList->append(heap.extractMax());
        }

        return sortedList;
    }

    LinkedList* readCSV(const string& filename, int columnIndex) {
        LinkedList* list = new LinkedList();
        ifstream file(filename);
        
        if (!file.is_open()) {
            cerr << "Error: Could not open file '" << filename << "'" << endl;
            return list;
        }
        
        string line;
        getline(file, line); // Skip header
        
        while (getline(file, line)) {
            stringstream ss(line);
            string value;
            int currentColumn = 0;
            
            while (getline(ss, value, ',')) {
                if (currentColumn == columnIndex) {
                    try {
                        double number = stod(value);
                        list->append(number);
                    } catch (...) {
                        cerr << "Error: Could not convert '" << value << "' to number" << endl;
                    }
                    break;
                }
                currentColumn++;
            }
        }
        
        return list;
    }

    void printList(LinkedList* list, const string& message = "") {
        if (!message.empty()) {
            cout << message << endl;
        }
        
        Node* current = list->getHead();
        while (current) {
            cout << fixed << setprecision(2) << current->data;
            if (current->next) cout << " -> ";
            current = current->next;
        }
        cout << endl;
    }

    void processTransactions(const string& filename) {
        string fullPath = "c:\\Users\\Keith Lo Ze Hui\\Desktop\\Deg 2 sem 2 assignment\\DataStruck\\data\\" + filename;
        cout << "\nProcessing Transactions Data for Heap Sort..." << endl;
        cout << "Reading from file: " << fullPath << endl;
        
        LinkedList* list = readCSV(fullPath, 2);
        
        if (list->getSize() == 0) {
            cout << "No data to process. Please check the file path and content." << endl;
            return;
        }
        
        cout << "Original amounts: ";
        printList(list);
        
        LinkedList* sortedList = sort(list);
        
        cout << "\nSorted amounts: ";
        printList(sortedList);
            
        cout << "\nStatistics:" << endl;
        cout << "Number of transactions: " << sortedList->getSize() << endl;
        cout << "Minimum amount: $" << sortedList->getHead()->data << endl;
        cout << "Maximum amount: $" << sortedList->getHead()->data << endl;
        
        delete list;
        delete sortedList;
    }
};

// Main function
int main() {
    KeithHeapSort sorter;
    sorter.processTransactions("transactionsClean.csv");
    return 0;
}
