#ifndef AMAL_HPP
#define AMAL_HPP

#include <string>
using namespace std;

struct Transaction{
    string customerID;
    string product;
    double price;
    string date;
    string category;
    string paymentMethod;
};

struct TransactionNode{
    Transaction data;
    TransactionNode* next;
    TransactionNode(Transaction t);
};

class TransactionLinkedListStore{
    private:
        TransactionNode* head;
        int size;
        int convertDateToInt(const string& dateStr);
        void sortedInsertByDate(TransactionNode*& sortedHead, TransactionNode* newNode);

    public:
        TransactionLinkedListStore();
        void insert(const Transaction& t);
        void insertionSortByDate();
        void displayTransactions(int limit = 10) const;
        TransactionNode* getHead() const;
        int getSize() const;
        ~TransactionLinkedListStore();
};

#endif