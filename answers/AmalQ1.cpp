#include <iostream>
#include "C:\Users\User\OneDrive - Asia Pacific University\DSA\DataStruck-1\include\AmalHPP.hpp"
#include <fstream>
#include <sstream>
using namespace std;

void loadTransactionsFromCSV(const string& filename, TransactionLinkedListStore& store){
    ifstream file(filename);
    string line;
    getline(file, line);

    while(getline(file, line)){
        stringstream ss(line);
        Transaction t;
        string token;

        getline(ss, t.customerID, ',');
        getline(ss, t.product, ',');
        getline(ss, token, ',');
        t.price = stod(token);
        getline(ss, t.date, ',');
        getline(ss, t.category, ',');
        getline(ss, t.paymentMethod);

        store.insert(t);
    }
}

void loadReviewsFromCSV(const string& filename, ReviewLinkedListStore& store) {
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        Review r;
        string token;

        getline(ss, r.productID, ',');
        getline(ss, r.customerID, ',');
        getline(ss, token, ',');
        r.rating = stoi(token);
        getline(ss, r.reviewText);

        store.insert(r);
    }
}

int main(){
    TransactionLinkedListStore transactions;
    loadTransactionsFromCSV("C:\\Users\\User\\OneDrive - Asia Pacific University\\DSA\\DataStruck-1\\data\\transactionsClean.csv", transactions);

    ReviewLinkedListStore reviews;
    loadReviewsFromCSV("C:\\Users\\User\\OneDrive - Asia Pacific University\\DSA\\DataStruck-1\\data\\reviewsClean.csv", reviews);

    cout << "====== QUESTION 1 ======\n\n";

    cout << "Total transactions loaded: " << transactions.getSize() << endl;
    cout << "Total reviews loaded: " << reviews.getSize() << endl;

    cout << "\nTransactions BEFORE sorting:\n";
    transactions.displayTransactions(10); // Optional to compare

    transactions.insertionSortByDate();

    cout << "\nTransactions AFTER sorting by date:\n";
    transactions.displayTransactions(10);


    return 0;
}