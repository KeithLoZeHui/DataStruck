#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <cctype>
using namespace std;

// Data strcutures: define Transaction and Review
struct Transaction {
    string customerID;
    string product;
    double price;
    string date;
    string category;
    string paymentMethod;
};

struct Review {
    string productID;
    string customerID;
    int rating;
    string reviewText;
};

// Helper: Convert string to lowercase and remove punctuation (Clean Words)
string cleanWord(string word) {
    string result;
    for (char c : word) {
        if (isalnum(c)) result += tolower(c);
    }
    return result;
}

// Quick sort comparator for sorting by date
bool compareByDate(const Transaction &a, const Transaction &b) {
    return a.date < b.date;
}

// Binary search for category
// Performs a binary search on a sorted list of transactions
// Check whether the category exists
bool binarySearchCategory(const vector<Transaction> &data, const string &category) {
    int left = 0, right = data.size() - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (data[mid].category == category)
            return true;
        else if (data[mid].category < category)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return false;
}

// Main Program logic
int main() {
    // Sample data transactions
    vector<Transaction> transactions = {
        {"C1", "Phone", 699.0, "2023-01-01", "Electronics", "Credit Card"},
        {"C2", "TV", 1200.0, "2023-02-15", "Electronics", "Debit Card"},
        {"C3", "Shoes", 100.0, "2023-01-10", "Fashion", "Credit Card"},
        {"C4", "Laptop", 999.0, "2023-03-05", "Electronics", "Credit Card"},
    };

    vector<Review> reviews = {
        {"P1", "C1", 1, "Terrible product, broke after one day"},
        {"P2", "C2", 5, "Excellent quality"},
        {"P3", "C3", 1, "Very bad experience, not recommended"},
    };

    // Sort transactions by date
    sort(transactions.begin(), transactions.end(), compareByDate);
    cout << "Sorted Transactions by Date:\n";
    // Display sorted transactions
    for (auto &t : transactions) {
        cout << t.date << " - " << t.product << "\n";
    }
    cout << "Total Transactions: " << transactions.size() << endl;
    cout << "Total Reviews: " << reviews.size() << endl;

    // Calculate Percentage of "Electronics" with "Credit Card"
    int electronicsTotal = 0, electronicsCreditCard = 0;
    for (auto &t : transactions) {
        if (t.category == "Electronics") {
            electronicsTotal++;
            if (t.paymentMethod == "Credit Card")
                electronicsCreditCard++;
        }
    }
    double percent = (electronicsTotal > 0) ? (electronicsCreditCard * 100.0 / electronicsTotal) : 0;
    cout << "\nCredit Card purchases in Electronics: " << percent << "%" << endl;

    // Word frequency in 1-star reviews
    map<string, int> wordFreq;
    for (auto &r : reviews) {
        if (r.rating == 1) {
            stringstream ss(r.reviewText);
            string word;
            while (ss >> word) {
                string cleaned = cleanWord(word);
                if (!cleaned.empty()) wordFreq[cleaned]++;
            }
        }
    }

    // Sort words by frequency 
    // Converts the map to a vector (so it can be sorted)
    // Most frequent first
    vector<pair<string, int>> sortedWords(wordFreq.begin(), wordFreq.end());
    sort(sortedWords.begin(), sortedWords.end(), [](auto &a, auto &b) {
        return b.second < a.second;
    });

    cout << "\nMost frequent words in 1-star reviews:\n";
    for (int i = 0; i < min(5, (int)sortedWords.size()); ++i) {
        cout << sortedWords[i].first << " - " << sortedWords[i].second << "\n";
    }

    return 0;
}
