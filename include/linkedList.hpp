#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

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

#endif // LINKEDLIST_HPP 