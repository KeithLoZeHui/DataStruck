
// Data Structures and Algorithms Assignment
// Author      : Keith Lo Ze Hui
// Student ID  : TP067653
// Description : Implementation of Heap Sort and Jump Search algorithms
// Note: In PowerShell, we need to use './' or '.\' 

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip> 
using namespace std;

class KeithSortingSearching {
private:
    void heapify(vector<double>& arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left] > arr[largest])
            largest = left;

        if (right < n && arr[right] > arr[largest])
            largest = right;

        if (largest != i) {
            swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }

    vector<double> readCSV(const string& filename, int columnIndex) {
        vector<double> values;
        ifstream file(filename);
        
        if (!file.is_open()) {
            cerr << "Error: Could not open file '" << filename << "'" << endl;
            return values;
        }
        
        string line;
        
        getline(file, line);
        
        while (getline(file, line)) {
            size_t pipePos = line.find('|');
            if (pipePos == string::npos) continue;
            
            string productInfo = line.substr(pipePos + 1);
            stringstream ss(productInfo);
            string value;
            int currentColumn = 0;
            
            while (getline(ss, value, ',')) {
                if (currentColumn == columnIndex) {
                    try {
                        double amount = stod(value);
                        values.push_back(amount);
                    } catch (...) {
                        cerr << "Warning: Could not convert value '" << value << "' to number" << endl;
                    }
                }
                currentColumn++;
            }
        }
        
        if (values.empty()) {
            cerr << "Warning: No valid numeric values found in column " << columnIndex << endl;
        } else {
            cout << "Successfully read " << values.size() << " values from the file." << endl;
        }
        
        return values;
    }

public:
    void heapSort(vector<double>& arr) {
        int n = arr.size();
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);

        for (int i = n - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
    }

    int jumpSearch(const vector<double>& arr, double x) {
        int n = arr.size();
        int step = sqrt(n);
        int prev = 0;
        
        while (arr[min(step, n) - 1] < x) {
            prev = step;
            step += sqrt(n);
            if (prev >= n)
                return -1;
        }
        
        while (arr[prev] < x) {
            prev++;
            if (prev == min(step, n))
                return -1;
        }
        
        if (arr[prev] == x)
            return prev;
        
        return -1;
    }

    void printArray(const vector<double>& arr, int limit = 10) {
        cout << "[ ";
        for (int i = 0; i < min((int)arr.size(), limit); i++) {
            cout << arr[i];
            if (i < min((int)arr.size() - 1, limit - 1))
                cout << ", ";
        }
        if (arr.size() > limit)
            cout << "... +" << arr.size() - limit << " more";
        cout << " ]" << endl;
    }

    void processTransactions(const string& filename) {
        cout << "\nProcessing Transactions Data..." << endl;
        cout << "Reading from file: " << "data/" + filename << endl;
        
        vector<double> amounts = readCSV("data/" + filename, 2);
        
        if (amounts.empty()) {
            cout << "No data to process. Please check the file path and content." << endl;
            return;
        }
        
        cout << "Original amounts: ";
        printArray(amounts);
        
        heapSort(amounts);
        cout << "Sorted amounts: ";
        printArray(amounts);
        
        double searchAmount = amounts[amounts.size() / 2];
        int result = jumpSearch(amounts, searchAmount);
        cout << "\nSearching for amount $" << fixed << setprecision(2) << searchAmount;
        if (result != -1)
            cout << "\nAmount $" << searchAmount << " found at index " << result << endl;
        else
            cout << "\nAmount $" << searchAmount << " not found in the dataset" << endl;
            
        cout << "\nStatistics:" << endl;
        cout << "Number of transactions: " << amounts.size() << endl;
        cout << "Minimum amount: $" << amounts.front() << endl;
        cout << "Maximum amount: $" << amounts.back() << endl;
    }
};

int main() {
    KeithSortingSearching sorter;
    
    sorter.processTransactions("transactionsClean.csv");
    
    return 0;
}
