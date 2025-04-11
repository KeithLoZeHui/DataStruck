#include "../../include/KeithHPP.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

// Node structures
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

// Linked List for strings
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
    
    void add(const std::string& str) {
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

// Linked List for transactions
class TransactionList {
private:
    TransactionNode* head;
    TransactionNode* tail;
    int size;

public:
    TransactionList() : head(nullptr), tail(nullptr), size(0) {}
    
    ~TransactionList() {
        while (head) {
            TransactionNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void add(const std::string& date, const std::string& customer, double price) {
        TransactionNode* newNode = new TransactionNode(date, customer, price);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }
    
    TransactionNode* getHead() const { return head; }
    int getSize() const { return size; }
    
    // Sort transactions by date using bubble sort
    void sortByDate() {
        if (!head || !head->next) return;
        
        bool swapped;
        TransactionNode* ptr1;
        TransactionNode* lptr = nullptr;
        
        do {
            swapped = false;
            ptr1 = head;
            
            while (ptr1->next != lptr) {
                if (ptr1->date > ptr1->next->date) {
                    // Swap data instead of nodes
                    std::swap(ptr1->date, ptr1->next->date);
                    std::swap(ptr1->customer, ptr1->next->customer);
                    std::swap(ptr1->price, ptr1->next->price);
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }
    
    // Calculate total price for a customer
    double calculateCustomerTotal(const std::string& customer) {
        double total = 0.0;
        TransactionNode* current = head;
        while (current) {
            if (current->customer == customer) {
                total += current->price;
            }
            current = current->next;
        }
        return total;
    }
    
    // Print all transactions
    void printTransactions() {
        TransactionNode* current = head;
        while (current) {
            std::cout << current->date << " | " 
                 << current->customer << " | $"
                 << std::fixed << std::setprecision(2) << current->price << std::endl;
            current = current->next;
        }
    }
};

// Function to split string by delimiter
StringList* split(const std::string& str, char delim) {
    StringList* result = new StringList();
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delim)) {
        if (!token.empty()) {
            result->add(token);
        }
    }
    
    return result;
}

// Function to parse a transaction line
void parseTransaction(const std::string& line, std::string& date, std::string& customer, double& price) {
    StringList* parts = split(line, '|');
    StringNode* current = parts->getHead();
    
    if (current) {
        date = current->data;
        current = current->next;
    }
    if (current) {
        customer = current->data;
        current = current->next;
    }
    if (current) {
        try {
            price = std::stod(current->data);
        } catch (...) {
            price = 0.0;
        }
    }
    
    delete parts;
}

int main() {
    std::ifstream transFile("../data/transactionsClean.csv");
    std::ifstream reviewFile("../data/reviewsClean.csv");
    
    if (!transFile.is_open() || !reviewFile.is_open()) {
        std::cout << "Error opening files!" << std::endl;
        return 1;
    }

    // Count transactions and reviews
    int transCount = 0;
    int reviewCount = 0;
    
    // Store first 5 transactions by date
    TransactionList transactions;

    std::string line;
    
    // Skip headers
    std::getline(transFile, line);
    std::getline(reviewFile, line);

    // Process transactions
    while (std::getline(transFile, line)) {
        std::string date, customer;
        double price;
        parseTransaction(line, date, customer, price);
        
        if (!date.empty() && !customer.empty()) {
            transactions.add(date, customer, price);
            transCount++;
        }
    }

    // Count reviews
    while (std::getline(reviewFile, line)) {
        reviewCount++;
    }

    // Sort transactions by date
    transactions.sortByDate();

    // Print results
    std::cout << "Question 1 Results:" << std::endl;
    std::cout << "Total number of transactions: " << transCount << std::endl;
    std::cout << "Total number of reviews: " << reviewCount << std::endl;
    
    std::cout << "\nFirst 5 transactions sorted by date:" << std::endl;
    TransactionNode* current = transactions.getHead();
    for (int i = 0; i < 5 && current; i++) {
        std::cout << current->date << " - " 
                 << current->customer << " - $" 
                 << std::fixed << std::setprecision(2) << current->price << std::endl;
        current = current->next;
    }

    transFile.close();
    reviewFile.close();
    return 0;
}
