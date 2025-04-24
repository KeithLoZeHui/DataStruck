#include "../../include/SortingAlgorithms.hpp"
#include "../../include/linkedList.hpp"

// Linked List Merge Sort Implementation
void SortingAlgorithms::mergeSortLL(TransactionNode*& head) {
    if (!head || !head->next) return;

    TransactionNode* left;
    TransactionNode* right;

    split(head, left, right);

    mergeSortLL(left);
    mergeSortLL(right);

    head = merge(left, right);
}

TransactionNode* SortingAlgorithms::merge(TransactionNode* left, TransactionNode* right) {
    if (!left) return right;
    if (!right) return left;

    TransactionNode* result = nullptr;

    if (left->date <= right->date) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

void SortingAlgorithms::split(TransactionNode* source, TransactionNode*& left, TransactionNode*& right) {
    if (!source || !source->next) {
        left = source;
        right = nullptr;
        return;
    }

    TransactionNode* slow = source;
    TransactionNode* fast = source->next;

    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    left = source;
    right = slow->next;
    slow->next = nullptr;
}

// Array Merge Sort Implementation
void SortingAlgorithms::mergeSortArray(TransactionArray& transactions) {
    if (transactions.size() <= 1) return;
    mergeSortArrayRecursive(transactions.getDataPtr(), 0, transactions.size() - 1);
}

void SortingAlgorithms::mergeSortArrayRecursive(TransactionData* arr, size_t left, size_t right) {
    if (left >= right) return;
    size_t mid = left + (right - left) / 2;
    mergeSortArrayRecursive(arr, left, mid);
    mergeSortArrayRecursive(arr, mid + 1, right);
    mergeArrays(arr, left, mid, right);
}

void SortingAlgorithms::mergeArrays(TransactionData* arr, size_t left, size_t mid, size_t right) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    TransactionData* L = new TransactionData[n1];
    TransactionData* R = new TransactionData[n2];

    for (size_t i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (size_t j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

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

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
} 