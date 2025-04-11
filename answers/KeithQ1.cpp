#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstring>

// Custom dynamic array implementation
template<typename T>
class DynamicArray {
private:
    T* data;
    int capacity;
    int size_;

public:
    DynamicArray() : data(nullptr), capacity(0), size_(0) {}
    
    ~DynamicArray() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (size_ >= capacity) {
            int newCapacity = (capacity == 0) ? 1 : capacity * 2;
            T* newData = new T[newCapacity];
            for (int i = 0; i < size_; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
        data[size_++] = value;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    int size() const {
        return size_;
    }
};

// Custom string array for splitting
class StringArray {
    private:
        char** data;
        int size_;

    public:
        StringArray() : data(nullptr), size_(0) {}
        
        ~StringArray() {
            for (int i = 0; i < size_; i++) {
                delete[] data[i];
            }
            delete[] data;
        }

        void add(const std::string& str) {
            char** newData = new char*[size_ + 1];
            for (int i = 0; i < size_; i++) {
                newData[i] = data[i];
            }
            newData[size_] = new char[str.length() + 1];
            strcpy(newData[size_], str.c_str());
            delete[] data;
            data = newData;
            size_++;
        }

        std::string get(int index) const {
            return std::string(data[index]);
        }

        int size() const {
            return size_;
        }
};

// Function to split string by delimiter
StringArray split(const std::string& str, char delim) {
    StringArray tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delim)) {
        tokens.add(token);
    }
    return tokens;
}

// Function to parse a transaction line
void parseTransaction(const std::string& line, std::string& date, std::string& customer, double& price) {
    // First split by comma
    StringArray fields = split(line, ',');
    if (fields.size() >= 4) {
        // Handle Customer|Product field
        StringArray custProd = split(fields.get(0), '|');
        if (custProd.size() >= 2) {
            customer = custProd.get(0);
            date = fields.get(3);
            try {
                price = std::stod(fields.get(2));
            } catch (const std::exception&) {
                price = 0.0;
            }
        }
    }
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
    struct Transaction {
        std::string customer;
        std::string date;
        double price;
    };
    DynamicArray<Transaction> transactions;

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
            Transaction t;
            t.customer = customer;
            t.date = date;
            t.price = price;
            transactions.push_back(t);
            transCount++;
        }
    }

    // Count reviews
    while (std::getline(reviewFile, line)) {
        reviewCount++;
    }

    // Sort transactions by date using bubble sort
    for (int i = 0; i < transactions.size() - 1; i++) {
        for (int j = 0; j < transactions.size() - i - 1; j++) {
            // Compare dates in format DD/MM/YYYY
            if (transactions[j].date > transactions[j + 1].date) {
                Transaction temp = transactions[j];
                transactions[j] = transactions[j + 1];
                transactions[j + 1] = temp;
            }
        }
    }

    // Print results
    std::cout << "Question 1 Results:" << std::endl;
    std::cout << "Total number of transactions: " << transCount << std::endl;
    std::cout << "Total number of reviews: " << reviewCount << std::endl;
    
    std::cout << "\nFirst 5 transactions sorted by date:" << std::endl;
    for (int i = 0; i < 5 && i < transactions.size(); i++) {
        std::cout << transactions[i].date << " - " 
                 << transactions[i].customer << " - $" 
                 << transactions[i].price << std::endl;
    }

    transFile.close();
    reviewFile.close();
    return 0;
}
