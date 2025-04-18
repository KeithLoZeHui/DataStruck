#ifndef KEITHHPP_HPP
#define KEITHHPP_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Keep structures potentially used elsewhere or defined here
struct Review {
    std::string productID;
    std::string customerID;
    int rating;
    std::string reviewText;
    Review* next;

    Review() : next(nullptr) {}
};

struct WordFrequency {
    std::string word;
    int frequency;
    WordFrequency* next;

    WordFrequency() : frequency(0), next(nullptr) {}
};

// Removed outdated Transaction struct definition (defined in keithAns.cpp as TransactionNode/TransactionData)

// Removed unused Heap Sort related structures and classes

// Removed outdated function declarations (definitions are in keithAns.cpp)

#endif // KEITHHPP_HPP
