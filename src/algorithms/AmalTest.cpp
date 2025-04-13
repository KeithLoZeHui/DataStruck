#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

//Define data structure for transactions and reviews
struct Transaction{
    string customerID;
    string product;
    double price;
    string date;
    string category;
    string paymentMethod;
};

struct Review{
    string productID;
    string customerID;
    int rating;
    string reviewText;
};

//Linked List-Based Implementation
struct TransactionNode{
    Transaction data;
    TransactionNode* next;

    TransactionNode(Transaction t) : data(t), next(nullptr){}
};

class TransactionLinkedListStore{
    private:
        TransactionNode* head;
        int size;

        int convertDateToInt(const string& dateStr){
            //Format expected: "DD/MM/YYYY"
            int day = stoi(dateStr.substr(0, 2));
            int month = stoi(dateStr.substr(3, 2));
            int year = stoi(dateStr.substr(6, 4));
        
            return year * 10000 + month * 100 + day;
        }

        void sortedInsertByDate(TransactionNode*& sortedHead, TransactionNode* newNode){
            int newDate = convertDateToInt(newNode->data.date);

            if (!sortedHead || convertDateToInt(sortedHead->data.date) > newDate){
                newNode->next = sortedHead;
                sortedHead = newNode;
            } else {
                TransactionNode* current = sortedHead;
                while (current->next && convertDateToInt(current->next->data.date) <= newDate){
                    current = current->next;
                }
                newNode->next = current->next;
                current->next = newNode;
            }
            
        }

    public:
        TransactionLinkedListStore() : head(nullptr), size(0){}

        void insert(const Transaction& t){
            TransactionNode* newNode = new TransactionNode(t);
            if (!head) head = newNode;
            else{
                TransactionNode* temp = head;
                while (temp->next) temp = temp->next;
                temp->next = newNode;
            }
            size++;
        }


    TransactionNode* getHead() const { return head;}
    int getSize() const { return size;}

    void insertionSortByDate(){
        if (!head || !head->next) return; // Already sorted or empty

        TransactionNode* sorted = nullptr;
        TransactionNode* current = head;

        while (current){
            TransactionNode* next = current->next;
            sortedInsertByDate(sorted, current);
            current = next;
        }

        head = sorted;
    }

    void displayTransactions(int limit = 10) const {
        TransactionNode* current = head;
        int count = 0;
        while (current && count < limit){
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

    ~TransactionLinkedListStore(){
        while(head){
            TransactionNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

struct ReviewNode{
    Review data;
    ReviewNode* next;

    ReviewNode(Review r) : data(r), next(nullptr){}
};

class ReviewLinkedListStore{
    private:
        ReviewNode* head;
        int size;

    public:
        ReviewLinkedListStore() : head(nullptr), size(0){}

        void insert(const Review& r){
            ReviewNode* newNode = new ReviewNode(r);
            if (!head) head = newNode;
            else{
                ReviewNode* temp = head;
                while (temp->next) temp = temp->next;
                temp->next = newNode;
            }
            size++;
        }

    ReviewNode* getHead() const { return head;}
    int getSize() const { return size;}

    void displayReviews(){
        ReviewNode* current = head;
        while (current){
            cout << current->data.productID << ", "
                 << current->data.customerID << ", "
                 << current->data.rating << ", "
                 << current->data.reviewText << endl;
            current = current->next;
        }
    }

    ~ReviewLinkedListStore(){
        while(head){
            ReviewNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

//Load data into structures
void loadTransactionsFromCSV(const string& filename, TransactionLinkedListStore& store){
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)){
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

void loadReviewsFromCSV(const string& filename, ReviewLinkedListStore& store){
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)){
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
    cout  << "Loaded " << transactions.getSize() << " transactions. " << endl;

    cout << "Transactions before sorting: " << endl;
    transactions.displayTransactions(); 

    transactions.insertionSortByDate();

    cout << "\nTransactions after sorting by date: " << endl;
    transactions.displayTransactions(100); 

    //transactions.displayTransactions();

    // ReviewLinkedListStore reviews;
    // loadReviewsFromCSV("C:\\Users\\User\\OneDrive - Asia Pacific University\\DSA\\DataStruck-1\\data\\reviewsClean.csv", reviews);
    // cout << "Loaded " << reviews.getSize() << " reviews. " << endl;

    // //reviews.displayReviews();

    // return 0;
}