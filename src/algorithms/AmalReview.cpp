#include "C:/Users/User/OneDrive - Asia Pacific University/DSA/DataStruck-1/include/AmalHPP.hpp"
#include <iostream>

//Should separate the review and transaction classes into their own files for better organization.

ReviewNode::ReviewNode(Review r) : data(r), next(nullptr) {}

ReviewLinkedListStore::ReviewLinkedListStore() : head(nullptr), size(0) {}

void ReviewLinkedListStore::insert(const Review& r) {
    ReviewNode* newNode = new ReviewNode(r);
    if (!head) head = newNode;
    else {
        ReviewNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
    size++;
}

ReviewNode* ReviewLinkedListStore::getHead() const { return head; }
int ReviewLinkedListStore::getSize() const { return size; }

void ReviewLinkedListStore::displayReviews() {
    ReviewNode* current = head;
    while (current) {
        cout << "ProductID: " << current->data.productID
             << ", CustomerID: " << current->data.customerID
             << ", Rating: " << current->data.rating
             << ", Review: " << current->data.reviewText << endl;
        current = current->next;
    }
}

ReviewLinkedListStore::~ReviewLinkedListStore() {
    while (head) {
        ReviewNode* temp = head;
        head = head->next;
        delete temp;
    }
}
