#ifndef KEITH_HPP
#define KEITH_HPP

#include <string>
#include <utility>

//===============================
// Common Node Structures
//===============================
struct StringNode {
    std::string data;
    StringNode* next;
    StringNode(const std::string& str) : data(str), next(nullptr) {}
};

struct TransactionNode {
    std::string date;
    std::string customer;
    double price;
    TransactionNode* next;
    TransactionNode(const std::string& d, const std::string& c, double p) 
        : date(d), customer(c), price(p), next(nullptr) {}
};

struct HeapNode {
    double data;
    HeapNode* next;
    HeapNode* left;
    HeapNode* right;
    HeapNode* parent;
    HeapNode(double value) : data(value), next(nullptr), left(nullptr), right(nullptr), parent(nullptr) {}
};

//===============================
// Q1 - Transaction Processing
//===============================
class StringList {
private:
    StringNode* head;
    StringNode* tail;
    int size;

public:
    StringList();
    ~StringList();
    void add(const std::string& str);
    StringNode* getHead() const;
    int getSize() const;
};

class TransactionList {
private:
    TransactionNode* head;
    TransactionNode* tail;
    int size;

public:
    TransactionList();
    ~TransactionList();
    void add(const std::string& date, const std::string& customer, double price);
    TransactionNode* getHead() const;
    int getSize() const;
    void sortByDate();
    double calculateCustomerTotal(const std::string& customer);
    void printTransactions();
};

//===============================
// Q2 - Payment Analysis
//===============================
class PaymentList {
private:
    TransactionNode* head;
    TransactionNode* tail;
    int size;

public:
    PaymentList();
    ~PaymentList();
    void add(const std::string& category, const std::string& paymentMethod);
    TransactionNode* getHead() const;
    int getSize() const;
    void analyzePaymentMethods() const;
    void printTransactions() const;
};

//===============================
// Heap Sort Implementation
//===============================
class LinkedList {
private:
    HeapNode* head;
    int size;

public:
    LinkedList();
    ~LinkedList();
    void append(double value);
    HeapNode* getHead() const;
    int getSize() const;
};

class HeapLinkedList {
private:
    HeapNode* root;
    HeapNode* last;
    int size;

    HeapNode* getParent(HeapNode* node);
    HeapNode* getLastParent();
    void swapValues(HeapNode* a, HeapNode* b);
    void heapifyUp(HeapNode* node);
    void heapifyDown(HeapNode* node);

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
    LinkedList* sort(LinkedList* list);
    LinkedList* readCSV(const std::string& filename, int columnIndex);
    void printList(LinkedList* list, const std::string& message = "");
};

//===============================
// Common Utility Functions
//===============================
StringList* split(const std::string& str, char delim);
void parseTransaction(const std::string& line, std::string& date, std::string& customer, double& price);
bool parsePayment(const std::string& line, std::string& category, std::string& paymentMethod);

#endif // KEITH_HPP
