#include "../../include/KeithHPP.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>  // For strcpy

using namespace std;

// StringList implementation
StringList::StringList() : head(nullptr), tail(nullptr), size(0) {}

StringList::~StringList() {
    while (head) {
        StringNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void StringList::add(const string& str) {
    StringNode* newNode = new StringNode(str);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

StringNode* StringList::getHead() const { return head; }
int StringList::getSize() const { return size; }

// TransactionList implementation
TransactionList::TransactionList() : head(nullptr), tail(nullptr), size(0) {}

TransactionList::~TransactionList() {
    while (head) {
        TransactionNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void TransactionList::add(const string& category, const string& paymentMethod) {
    TransactionNode* newNode = new TransactionNode(category, paymentMethod);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

TransactionNode* TransactionList::getHead() const { return head; }
int TransactionList::getSize() const { return size; }

void TransactionList::printTransactions() const {
    TransactionNode* current = head;
    while (current) {
        cout << current->category << " | " << current->paymentMethod << endl;
        current = current->next;
    }
}

void TransactionList::analyzePaymentMethods() const {
    // Create a linked list to store unique payment methods
    StringList paymentMethods;
    int* counts = new int[100](); // Assuming max 100 unique payment methods

    // Count occurrences of each payment method
    TransactionNode* current = head;
    while (current) {
        // Check if payment method already exists
        bool found = false;
        StringNode* pmNode = paymentMethods.getHead();
        int index = 0;
        
        while (pmNode) {
            if (pmNode->data == current->paymentMethod) {
                counts[index]++;
                found = true;
                break;
            }
            pmNode = pmNode->next;
            index++;
        }
        
        if (!found) {
            paymentMethods.add(current->paymentMethod);
            counts[paymentMethods.getSize() - 1] = 1;
        }
        
        current = current->next;
    }

    // Print analysis
    StringNode* pmNode = paymentMethods.getHead();
    int index = 0;
    while (pmNode) {
        cout << pmNode->data << ": " << counts[index] << " transactions" << endl;
        pmNode = pmNode->next;
        index++;
    }

    delete[] counts;
}

// Function to split string by delimiter
StringList* split(const string& str, char delim) {
    StringList* result = new StringList();
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, delim)) {
        if (!token.empty()) {
            result->add(token);
        }
    }
    
    return result;
}

// Function to parse a transaction line
bool parseTransaction(const string& line, string& category, string& paymentMethod) {
    StringList* fields = split(line, ',');
    StringNode* current = fields->getHead();
    
    if (fields->getSize() >= 5 && current) {
        StringList* custProd = split(current->data, '|');
        StringNode* prodNode = custProd->getHead();
        
        if (custProd->getSize() >= 2 && prodNode && prodNode->next) {
            category = prodNode->next->data;
            
            // Move to payment method field (4th field)
            for (int i = 0; i < 4 && current; i++) {
                current = current->next;
            }
            
            if (current) {
                paymentMethod = current->data;
                delete custProd;
                delete fields;
                return true;
            }
        }
        delete custProd;
    }
    delete fields;
    return false;
}

int main() {
    std::ifstream file("../data/transactionsClean.csv");
    if (!file.is_open()) {
        std::cout << "Error opening file!" << std::endl;
        return 1;
    }

    int totalElectronics = 0;
    int electronicsWithCC = 0;

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::string category, paymentMethod;
        if (parseTransaction(line, category, paymentMethod)) {
            if (category == "Electronics") {
                totalElectronics++;
                if (paymentMethod == "Credit Card") {
                    electronicsWithCC++;
                }
            }
        }
    }

    std::cout << "Question 2 Results:" << std::endl;
    std::cout << "Total Electronics purchases: " << totalElectronics << std::endl;
    std::cout << "Electronics purchases with Credit Card: " << electronicsWithCC << std::endl;
    
    double percentage = (totalElectronics > 0) ? 
        (electronicsWithCC * 100.0 / totalElectronics) : 0.0;
    
    std::cout << "Percentage: " << std::fixed << std::setprecision(2) 
              << percentage << "%" << std::endl;

    file.close();
    return 0;
}
