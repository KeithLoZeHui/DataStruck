#ifndef SORTING_ALGORITHMS_HPP
#define SORTING_ALGORITHMS_HPP

#include <string>
#include <chrono>

// Forward declarations
struct TransactionNode;
struct TransactionData;
class TransactionArray;

class SortingAlgorithms {
public:
    // Linked List Merge Sort
    static void mergeSortLL(TransactionNode*& head);
    
    // Array Merge Sort
    static void mergeSortArray(TransactionArray& transactions);
    
    // Helper functions for Linked List
    static TransactionNode* merge(TransactionNode* left, TransactionNode* right);
    static void split(TransactionNode* source, TransactionNode*& left, TransactionNode*& right);
    
    // Helper functions for Array
    static void mergeArrays(TransactionData* arr, size_t left, size_t mid, size_t right);
    static void mergeSortArrayRecursive(TransactionData* arr, size_t left, size_t right);
    
    // Performance measurement
    template<typename Func>
    static double measureSortTime(Func sortFunction) {
        auto start = std::chrono::high_resolution_clock::now();
        sortFunction();
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};

#endif // SORTING_ALGORITHMS_HPP 