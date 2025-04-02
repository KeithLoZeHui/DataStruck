#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Review {
    string productID, customerID, reviewText;
    int rating;
};

class ReviewAnalyzer {
private:
    unordered_map<string, int> wordFrequency;

    vector<string> tokenizeText(const string& text) {
        vector<string> tokens;
        string word;
        stringstream ss(text);
        
        while (ss >> word) {
            string cleanWord;
            for (char c : word) {
                if (isalnum(c)) {
                    cleanWord += tolower(c);
                }
            }
            
            if (cleanWord.length() > 2) {
                tokens.push_back(cleanWord);
            }
        }
        return tokens;
    }

    void processReview(const Review& review) {
        if (review.rating == 1) {
            vector<string> tokens = tokenizeText(review.reviewText);
            for (const auto& token : tokens) {
                wordFrequency[token]++;
            }
        }
    }

public:
    void analyzeReviews(const vector<Review>& reviews) {
        for (const auto& review : reviews) {
            processReview(review);
        }
    }

    vector<pair<string, int>> getTopWords(int n) {
        priority_queue<pair<int, string>, 
                      vector<pair<int, string>>, 
                      greater<pair<int, string>>> pq;

        for (const auto& pair : wordFrequency) {
            pq.push({pair.second, pair.first});
            if (pq.size() > n) {
                pq.pop();
            }
        }

        vector<pair<string, int>> result;
        while (!pq.empty()) {
            result.push_back({pq.top().second, pq.top().first});
            pq.pop();
        }
        reverse(result.begin(), result.end());
        
        return result;
    }

    void printResults(int topN = 10) {
        cout << "\nTop " << topN << " most frequent words in 1-star reviews:\n";
        cout << "----------------------------------------\n";
        auto topWords = getTopWords(topN);
        
        for (const auto& pair : topWords) {
            cout << pair.first << ": " << pair.second << " occurrences\n";
        }
    }
};

vector<Review> loadReviews(const string& filename) {
    vector<Review> reviews;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Error: Cannot open " << filename << endl;
        return reviews;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        Review review;
        string rating;

        getline(ss, review.productID, ',');
        getline(ss, review.customerID, ',');
        getline(ss, rating, ',');
        getline(ss, review.reviewText);

        try {
            review.rating = stoi(rating);
            if (review.rating >= 1 && review.rating <= 5) {
                reviews.push_back(review);
            }
        } catch (...) {
            continue;
        }
    }

    return reviews;
}

int main() {
    vector<Review> reviews = loadReviews("data/reviewsClean.csv");
    
    if (reviews.empty()) {
        cout << "No reviews loaded. Exiting...\n";
        return 1;
    }
    
    ReviewAnalyzer analyzer;
    analyzer.analyzeReviews(reviews);
    analyzer.printResults(10);
    
    return 0;
}
