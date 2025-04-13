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

int main(){
    TransactionLinkedListStore transactions;
    loadTransactionsFromCSV("C:\\Users\\User\\OneDrive - Asia Pacific University\\DSA\\DataStruck-1\\data\\transactionsClean.csv", transactions);

    cout << "Before sorting:" << endl;
    transactions.displayTransactions(10); // Display first 10 transactions

    transactions.insertionSortByDate();

    cout << "\nAfter sorting by date:" << endl;
    transactions.displayTransactions(10); // Display first 10 transactions

    return 0;
}