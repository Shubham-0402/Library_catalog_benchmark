#ifndef BST_H
#define BST_H

#include "Book.h"
#include <algorithm>
#include <functional>
#include <iomanip>

/**
 * @struct BSTNode
 * @brief Node representation inside the Binary Search Tree.
 */
struct BSTNode {
    Book data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Book& book) : data(book), left(nullptr), right(nullptr) {}
};

/**
 * @class BST
 * @brief Standard (unbalanced) Binary Search Tree implementation.
 */
class BST {
private:
    BSTNode* root;
    int nodeCount;

    // Recursive helper for insertion
    BSTNode* insertHelper(BSTNode* node, const Book& book) {
        if (!node) {
            nodeCount++;
            return new BSTNode(book);
        }
        if (book.bookId < node->data.bookId) {
            node->left = insertHelper(node->left, book);
        } else if (book.bookId > node->data.bookId) {
            node->right = insertHelper(node->right, book);
        }
        // Duplicate keys are ignored in BST
        return node;
    }

    // Recursive helper for search with key comparison counting
    bool searchHelper(BSTNode* node, int bookId, Book& result, int& comparisons) const {
        if (!node) {
            return false;
        }

        comparisons++;
        if (node->data.bookId == bookId) {
            result = node->data;
            return true;
        }

        if (bookId < node->data.bookId) {
            return searchHelper(node->left, bookId, result, comparisons);
        } else {
            return searchHelper(node->right, bookId, result, comparisons);
        }
    }

    // Recursive helper for in-order traversal
    void inOrderHelper(BSTNode* node, const std::function<void(const Book&)>& visit) const {
        if (!node) return;
        inOrderHelper(node->left, visit);
        visit(node->data);
        inOrderHelper(node->right, visit);
    }

    // Recursive helper for tree height calculation
    int getHeightHelper(BSTNode* node) const {
        if (!node) return 0;
        return 1 + std::max(getHeightHelper(node->left), getHeightHelper(node->right));
    }

    // Recursive helper for deallocation
    void destroyTree(BSTNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    BST() : root(nullptr), nodeCount(0) {}

    ~BST() {
        destroyTree(root);
        root = nullptr;
        nodeCount = 0;
    }

    // Disable copy constructor and assignment operator to prevent double free
    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    /**
     * @brief Inserts a new book record into the BST.
     */
    void insert(const Book& book) {
        root = insertHelper(root, book);
    }

    /**
     * @brief Searches for a book by BookID, counting the number of node comparisons.
     * @param bookId The ID to search for
     * @param result Output parameter to receive the found Book record
     * @param comparisons Output parameter incremented on each key comparison
     * @return true if found, false otherwise
     */
    bool search(int bookId, Book& result, int& comparisons) const {
        comparisons = 0;
        return searchHelper(root, bookId, result, comparisons);
    }

    /**
     * @brief In-order traversal executing a callback on each book.
     */
    void inOrder(const std::function<void(const Book&)>& visit) const {
        inOrderHelper(root, visit);
    }

    /**
     * @brief Prints in-order traversal of all books in a formatted table.
     */
    void printInOrder() const {
        std::cout << "\n+--------+------------------------------------------------------+------------------------------+---------------------------+\n";
        std::cout << "| BookID | Title                                                | Author                       | Genre                     |\n";
        std::cout << "+--------+------------------------------------------------------+------------------------------+---------------------------+\n";
        inOrder([](const Book& b) {
            std::cout << "| " << std::left << std::setw(6) << b.bookId
                      << " | " << std::setw(52) << (b.title.length() > 52 ? b.title.substr(0, 49) + "..." : b.title)
                      << " | " << std::setw(28) << (b.author.length() > 28 ? b.author.substr(0, 25) + "..." : b.author)
                      << " | " << std::setw(25) << (b.genre.length() > 25 ? b.genre.substr(0, 22) + "..." : b.genre)
                      << " |\n";
        });
        std::cout << "+--------+------------------------------------------------------+------------------------------+---------------------------+\n";
    }

    /**
     * @brief Returns the maximum height of the BST.
     */
    int getHeight() const {
        return getHeightHelper(root);
    }

    /**
     * @brief Returns the total number of books stored in the BST.
     */
    int getNodeCount() const {
        return nodeCount;
    }
};

#endif // BST_H
