#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <cstring>
#include <iostream>
#include <stdexcept>

// Custom string implementation to avoid using STL containers
class MyString {
private:
    char* data;
    size_t length;

public:
    MyString() : data(nullptr), length(0) {
        data = new char[1];
        data[0] = '\0';
    }

    MyString(const char* str) : data(nullptr), length(0) {
        if (str) {
            length = strlen(str);
            data = new char[length + 1];
            strcpy(data, str);
        } else {
            data = new char[1];
            data[0] = '\0';
        }
    }

    MyString(const MyString& other) : data(nullptr), length(other.length) {
        data = new char[length + 1];
        strcpy(data, other.data);
    }

    ~MyString() {
        delete[] data;
    }

    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    bool operator<=(const MyString& other) const {
        return strcmp(data, other.data) <= 0;
    }

    bool operator==(const MyString& other) const {
        return strcmp(data, other.data) == 0;
    }

    const char* c_str() const { return data; }
    size_t size() const { return length; }
    bool empty() const { return length == 0; }

    friend std::ostream& operator<<(std::ostream& os, const MyString& str) {
        os << str.data;
        return os;
    }
};

// Keep structures potentially used elsewhere or defined here
struct Review {
    MyString productID;
    MyString customerID;
    int rating;
    MyString reviewText;
    Review* next;

    Review() : rating(0), next(nullptr) {}
};

struct WordFrequency {
    MyString word;
    int frequency;
    WordFrequency* next;

    WordFrequency() : frequency(0), next(nullptr) {}
};

struct TransactionNode {
    MyString customerID;
    MyString product;
    MyString category;
    double price;
    MyString date;
    MyString paymentMethod;
    TransactionNode* next;

    TransactionNode() : price(0.0), next(nullptr) {}
};

// Array implementation (no next pointer)
struct TransactionData {
    MyString customerID;
    MyString product;
    MyString category;
    double price;
    MyString date;
    MyString paymentMethod;

    TransactionData() : price(0.0) {}
};

// Custom Dynamic Array Implementation for TransactionData
class TransactionArray {
private:
    TransactionData* data;
    size_t arraySize;
    size_t arrayCapacity;

    void resize(size_t newCapacity) {
        if (newCapacity <= arrayCapacity) return;

        TransactionData* newData = new TransactionData[newCapacity];
        for (size_t i = 0; i < arraySize; ++i) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        arrayCapacity = newCapacity;
    }

public:
    TransactionArray() : data(nullptr), arraySize(0), arrayCapacity(0) {
        resize(10);
    }

    ~TransactionArray() {
        delete[] data;
    }

    void push_back(const TransactionData& transaction) {
        if (arraySize == arrayCapacity) {
            resize(arrayCapacity == 0 ? 10 : arrayCapacity * 2);
        }
        data[arraySize++] = transaction;
    }

    const TransactionData& at(size_t index) const {
        if (index >= arraySize) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    TransactionData& at(size_t index) {
        if (index >= arraySize) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const TransactionData& operator[](size_t index) const {
        return data[index];
    }

    TransactionData& operator[](size_t index) {
        return data[index];
    }

    size_t size() const {
        return arraySize;
    }

    bool empty() const {
        return arraySize == 0;
    }

    TransactionData* getDataPtr() { return data; }
    const TransactionData* getDataPtr() const { return data; }
};

#endif // LINKEDLIST_HPP 