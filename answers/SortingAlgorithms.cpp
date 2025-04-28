#include "../include/SortingAlgorithms.hpp"
#include "../include/linkedList.hpp"

// Linked List Merge Sort implementation
void SortingAlgorithms::mergeSortLL(TransactionNode*& head) {
    // Base case
    if (!head || !head->next) return;
    
    // Split the list into two halves
    TransactionNode* left = nullptr;
    TransactionNode* right = nullptr;
    split(head, left, right);
    
    // Recursively sort both halves
    mergeSortLL(left);
    mergeSortLL(right);
    
    // Merge the sorted halves
    head = merge(left, right);
}

// Helper function for Linked List Merge Sort
TransactionNode* SortingAlgorithms::merge(TransactionNode* left, TransactionNode* right) {
    TransactionNode* result = nullptr;
    
    // Base cases
    if (!left) return right;
    if (!right) return left;
    
    // Compare dates and merge
    if (left->date <= right->date) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }
    
    return result;
}

// Helper function for Linked List Merge Sort
void SortingAlgorithms::split(TransactionNode* source, TransactionNode*& left, TransactionNode*& right) {
    if (!source || !source->next) {
        left = source;
        right = nullptr;
        return;
    }
    
    // Use slow and fast pointers to find the middle
    TransactionNode* slow = source;
    TransactionNode* fast = source->next;
    
    // Fast moves twice as fast as slow
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    
    // Split the list into two halves
    left = source;
    right = slow->next;
    slow->next = nullptr; // Terminate first half
}

// Array Merge Sort implementation
void SortingAlgorithms::mergeSortArray(TransactionArray& transactions) {
    if (transactions.size() > 1) {
        mergeSortArrayRecursive(transactions.getDataPtr(), 0, transactions.size() - 1);
    }
}

// Helper function for Array Merge Sort
void SortingAlgorithms::mergeArrays(TransactionData* arr, size_t left, size_t mid, size_t right) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;
    
    // Create temporary arrays
    TransactionData* L = new TransactionData[n1];
    TransactionData* R = new TransactionData[n2];
    
    // Copy data to temporary arrays L[] and R[]
    for (size_t i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (size_t j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    // Merge the temporary arrays back into arr[left..right]
    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].date <= R[j].date) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    // Free temporary arrays
    delete[] L;
    delete[] R;
}

// Helper function for Array Merge Sort
void SortingAlgorithms::mergeSortArrayRecursive(TransactionData* arr, size_t left, size_t right) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        
        // Sort first and second halves
        mergeSortArrayRecursive(arr, left, mid);
        mergeSortArrayRecursive(arr, mid + 1, right);
        
        // Merge the sorted halves
        mergeArrays(arr, left, mid, right);
    }
}
