#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

// Custom dynamic array template
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

// Word frequency counter structure
struct WordFreq {
    char* word;
    int count;

    WordFreq() : word(nullptr), count(0) {}
    
    WordFreq(const std::string& w, int c) : count(c) {
        word = new char[w.length() + 1];
        strcpy(word, w.c_str());
    }

    ~WordFreq() {
        delete[] word;
    }

    // Copy constructor
    WordFreq(const WordFreq& other) : count(other.count) {
        if (other.word) {
            word = new char[strlen(other.word) + 1];
            strcpy(word, other.word);
        } else {
            word = nullptr;
        }
    }

    // Assignment operator
    WordFreq& operator=(const WordFreq& other) {
        if (this != &other) {
            delete[] word;
            count = other.count;
            if (other.word) {
                word = new char[strlen(other.word) + 1];
                strcpy(word, other.word);
            } else {
                word = nullptr;
            }
        }
        return *this;
    }
};

// Function to split string by comma
StringArray split(const std::string& str) {
    StringArray tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, ',')) {
        tokens.add(token);
    }
    return tokens;
}

// Function to convert string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

// Function to check if a word is a stop word
bool isStopWord(const std::string& word) {
    const char* stopWords[] = {
        "the", "a", "an", "and", "or", "but", "in", "on", "at", "to",
        "for", "is", "are", "was", "were", "i", "you", "he", "she", "it",
        "we", "they", "this", "that", "have", "has", "had", "be"
    };
    const int numStopWords = sizeof(stopWords) / sizeof(stopWords[0]);
    
    for (int i = 0; i < numStopWords; i++) {
        if (word == stopWords[i]) {
            return true;
        }
    }
    return false;
}

// Function to clean word (remove punctuation and convert to lowercase)
std::string cleanWord(const std::string& word) {
    std::string cleaned;
    for (char c : word) {
        if (std::isalnum(c)) {
            cleaned += std::tolower(c);
        }
    }
    return cleaned;
}

int main() {
    std::ifstream file("../data/reviewsClean.csv");
    if (!file.is_open()) {
        std::cout << "Error opening file!" << std::endl;
        return 1;
    }

    DynamicArray<WordFreq> wordFrequency;
    std::string line;

    // Skip header
    std::getline(file, line);

    // Process each review
    while (std::getline(file, line)) {
        StringArray fields = split(line);
        if (fields.size() >= 4) {
            try {
                int rating = std::stoi(fields.get(2));
                if (rating == 1) {  // Only process 1-star reviews
                    std::string review = fields.get(3);
                    std::istringstream words(review);
                    std::string word;
                    
                    // Split review into words
                    while (words >> word) {
                        word = cleanWord(word);
                        if (word.length() > 2 && !isStopWord(word)) {
                            // Check if word exists
                            bool found = false;
                            for (int i = 0; i < wordFrequency.size(); i++) {
                                if (strcmp(wordFrequency[i].word, word.c_str()) == 0) {
                                    wordFrequency[i].count++;
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                wordFrequency.push_back(WordFreq(word, 1));
                            }
                        }
                    }
                }
            } catch (const std::exception&) {
                continue;  // Skip invalid ratings
            }
        }
    }

    // Sort by frequency using bubble sort
    for (int i = 0; i < wordFrequency.size() - 1; i++) {
        for (int j = 0; j < wordFrequency.size() - i - 1; j++) {
            if (wordFrequency[j].count < wordFrequency[j + 1].count) {
                WordFreq temp = wordFrequency[j];
                wordFrequency[j] = wordFrequency[j + 1];
                wordFrequency[j + 1] = temp;
            }
        }
    }

    // Print top 10 most frequent words
    std::cout << "Question 3 Results:" << std::endl;
    std::cout << "Most frequent words in 1-star reviews:" << std::endl;
    for (int i = 0; i < 10 && i < wordFrequency.size(); i++) {
        std::cout << wordFrequency[i].word << ": " 
                 << wordFrequency[i].count << " times" << std::endl;
    }

    file.close();
    return 0;
}
