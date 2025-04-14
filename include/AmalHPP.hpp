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

struct Review{
    string productID;
    string customerID;
    double rating;
    string reviewText;
};

struct ReviewNode{
    Review data;
    ReviewNode* next;
    ReviewNode(Review r);
};

class ReviewLinkedListStore{
    private:
        ReviewNode* head;
        int size;

    public:
        ReviewLinkedListStore();
        void insert(const Review& r);
        ReviewNode* getHead() const;
        int getSize() const;
        void displayReviews();
        ~ReviewLinkedListStore();
};

#endif