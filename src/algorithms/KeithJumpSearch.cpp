// Data Structures and Algorithms Assignment
// Author      : Keith Lo Ze Hui
// Student ID  : TP067653
// Description : Implementation of Jump Search algorithm

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip> 
using namespace std;

struct Node {
    double data;
    Node* next;
    Node(double value) : data(value), next(nullptr) {}
};

class KeithJumpSearch {
private:
    Node* readCSV(const string& filename, int columnIndex) {
        Node* head = nullptr;
        ifstream file(filename);
        
        if (!file.is_open()) {
            cerr << "Error: Could not open file '" << filename << "'" << endl;
            return head;
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
                        Node* newNode = new Node(amount);
                        if (head == nullptr) {
                            head = newNode;
                        } else {
                            Node* temp = head;
                            while (temp->next != nullptr) {
                                temp = temp->next;
                            }
                            temp->next = newNode;
                        }
                    } catch (...) {
                        cerr << "Warning: Could not convert value '" << value << "' to number" << endl;
                    }
                }
                currentColumn++;
            }
        }
        
        if (head == nullptr) {
            cerr << "Warning: No valid numeric values found in column " << columnIndex << endl;
        } else {
            cout << "Successfully read values from the file." << endl;
        }
        
        return head;
    }

    void insertionSort(double arr[], int n) {
        for (int i = 1; i < n; i++) {
            double key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }

public:
    int jumpSearch(double arr[], int n, double x) {
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

    void printArray(double arr[], int n, int limit = 10) {
        cout << "[ ";
        for (int i = 0; i < min(n, limit); i++) {
            cout << arr[i];
            if (i < min(n - 1, limit - 1))
                cout << ", ";
        }
        if (n > limit)
            cout << "... +" << n - limit << " more";
        cout << " ]" << endl;
    }

    void processTransactions(const string& filename) {
        string fullPath = "c:\\Users\\Keith Lo Ze Hui\\Desktop\\Deg 2 sem 2 assignment\\DataStruck\\data\\" + filename;
        cout << "\nProcessing Transactions Data for Jump Search..." << endl;
        cout << "Reading from file: " << fullPath << endl;
        
        Node* head = readCSV(fullPath, 2);
        
        if (head == nullptr) {
            cout << "No data to process. Please check the file path and content." << endl;
            return;
        }
        
        // Convert linked list to array and count elements
        int n = 0;
        Node* temp = head;
        while (temp != nullptr) {
            n++;
            temp = temp->next;
        }
        
        double* arr = new double[n];
        temp = head;
        for (int i = 0; i < n; i++) {
            arr[i] = temp->data;
            temp = temp->next;
        }
        
        // Sort array using insertion sort (required for jump search)
        insertionSort(arr, n);
        cout << "Sorted amounts: ";
        printArray(arr, n);
        
        // Search for median value
        double searchAmount = arr[n / 2];
        int result = jumpSearch(arr, n, searchAmount);
        
        cout << "\nJump Search Results:" << endl;
        cout << "Searching for median amount: $" << fixed << setprecision(2) << searchAmount << endl;
        if (result != -1)
            cout << "Amount found at index: " << result << endl;
        else
            cout << "Amount not found in the dataset" << endl;
        
        delete[] arr;
        
        // Clean up linked list
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    KeithJumpSearch searcher;
    searcher.processTransactions("transactionsClean.csv");
    return 0;
}
