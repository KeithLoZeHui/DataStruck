#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>  // For strcpy

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
bool parseTransaction(const std::string& line, std::string& category, std::string& paymentMethod) {
    // First split by comma
    StringArray fields = split(line, ',');
    if (fields.size() >= 5) {
        // Handle Customer|Product field
        StringArray custProd = split(fields.get(0), '|');
        if (custProd.size() >= 2) {
            category = fields.get(1);
            paymentMethod = fields.get(4);
            return true;
        }
    }
    return false;
}

int main() {
    std::ifstream file("../data/transactionsClean.csv");
    if (!file.is_open()) {
        std::cout << "Error opening file!" << std::endl;
        return 1;
    }

    int totalElectronics = 0;
    int electronicsWithCC = 0;

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::string category, paymentMethod;
        if (parseTransaction(line, category, paymentMethod)) {
            if (category == "Electronics") {
                totalElectronics++;
                if (paymentMethod == "Credit Card") {
                    electronicsWithCC++;
                }
            }
        }
    }

    std::cout << "Question 2 Results:" << std::endl;
    std::cout << "Total Electronics purchases: " << totalElectronics << std::endl;
    std::cout << "Electronics purchases with Credit Card: " << electronicsWithCC << std::endl;
    
    double percentage = (totalElectronics > 0) ? 
        (electronicsWithCC * 100.0 / totalElectronics) : 0.0;
    
    std::cout << "Percentage: " << std::fixed << std::setprecision(2) 
              << percentage << "%" << std::endl;

    file.close();
    return 0;
}
