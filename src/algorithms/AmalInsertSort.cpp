#include "C:\Users\User\OneDrive - Asia Pacific University\DSA\DataStruck-1\include\AmalHPP.hpp"
#include <iostream>
using namespace std;

// Node constructor
TransactionNode::TransactionNode(Transaction t) : data(t), next(nullptr) {}

TransactionLinkedListStore::TransactionLinkedListStore() : head(nullptr), size(0) {}

int TransactionLinkedListStore::convertDateToInt(const string& dateStr) {
    int day = stoi(dateStr.substr(0, 2));
    int month = stoi(dateStr.substr(3, 2));
    int year = stoi(dateStr.substr(6, 4));
    return year * 10000 + month * 100 + day;
}

void TransactionLinkedListStore::sortedInsertByDate(TransactionNode*& sortedHead, TransactionNode* newNode) {
    int newDate = convertDateToInt(newNode->data.date);
    if (!sortedHead || convertDateToInt(sortedHead->data.date) > newDate) {
        newNode->next = sortedHead;
        sortedHead = newNode;
    } else {
        TransactionNode* current = sortedHead;
        while (current->next && convertDateToInt(current->next->data.date) <= newDate) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

void TransactionLinkedListStore::insert(const Transaction& t) {
    TransactionNode* newNode = new TransactionNode(t);
    if (!head) head = newNode;
    else {
        TransactionNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
    size++;
}

void TransactionLinkedListStore::insertionSortByDate() {
    if (!head || !head->next) return;
    TransactionNode* sorted = nullptr;
    TransactionNode* current = head;
    while (current) {
        TransactionNode* next = current->next;
        sortedInsertByDate(sorted, current);
        current = next;
    }
    head = sorted;
}

void TransactionLinkedListStore::displayTransactions(int limit) const {
    TransactionNode* current = head;
    int count = 0;
    while (current && count < limit) {
        cout << "CustomerID: " << current->data.customerID << ", "
             << "Product: " << current->data.product << ", "
             << "Price: " << current->data.price << ", "
             << "Date: " << current->data.date << ", "
             << "Category: " << current->data.category << ", "
             << "Payment Method: " << current->data.paymentMethod << endl;
        current = current->next;
        count++;
    }
}

TransactionNode* TransactionLinkedListStore::getHead() const { return head; }
int TransactionLinkedListStore::getSize() const { return size; }

TransactionLinkedListStore::~TransactionLinkedListStore() {
    while (head) {
        TransactionNode* temp = head;
        head = head->next;
        delete temp;
    }
}
