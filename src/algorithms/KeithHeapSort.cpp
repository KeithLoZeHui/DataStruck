// Data Structures and Algorithms Assignment
// Author      : Keith Lo Ze Hui
// Student ID  : TP067653
// Description : Implementation of Heap Sort algorithm

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

class KeithHeapSort {
private:
    void heapify(double arr[], int n, int i) {
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

public:
    void heapSort(double arr[], int n) {
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);

        for (int i = n - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
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

    void printLinkedList(Node* head, int limit = 10) {
        cout << "[ ";
        int count = 0;
        while (head != nullptr && count < limit) {
            cout << head->data;
            if (head->next != nullptr && count < limit - 1)
                cout << ", ";
            head = head->next;
            count++;
        }
        if (count >= limit)
            cout << "... +" << (count - limit) << " more";
        cout << " ]" << endl;
    }

    void processTransactions(const string& filename) {
        string fullPath = "c:\\Users\\Keith Lo Ze Hui\\Desktop\\Deg 2 sem 2 assignment\\DataStruck\\data\\" + filename;
        cout << "\nProcessing Transactions Data for Heap Sort..." << endl;
        cout << "Reading from file: " << fullPath << endl;
        
        Node* head = readCSV(fullPath, 2);
        
        if (head == nullptr) {
            cout << "No data to process. Please check the file path and content." << endl;
            return;
        }
        
        cout << "Original amounts: ";
        printLinkedList(head);
        
        // Convert linked list to array
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
        
        heapSort(arr, n);
        cout << "Sorted amounts: ";
        printArray(arr, n);
            
        cout << "\nStatistics:" << endl;
        cout << "Number of transactions: " << n << endl;
        cout << "Minimum amount: $" << fixed << setprecision(2) << arr[0] << endl;
        cout << "Maximum amount: $" << arr[n - 1] << endl;
        
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
    KeithHeapSort sorter;
    sorter.processTransactions("transactionsClean.csv");
    return 0;
}
