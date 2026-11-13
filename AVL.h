#ifndef AVL_H
#define AVL_H

#include "Book.h"
#include <algorithm>
#include <functional>
#include <iomanip>

/**
 * @struct AVLNode
 * @brief Node representation inside the self-balancing AVL Tree.
 */
struct AVLNode {
    Book data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const Book& book)
        : data(book), left(nullptr), right(nullptr), height(1) {}
};

/**
 * @class AVL
 * @brief Self-Balancing Binary Search Tree adhering to the Adelson-Velsky and Landis invariant.
 */
class AVL {
private:
    AVLNode* root;
    int nodeCount;

    // Rotation telemetry counters
    int rotLLCount;
    int rotRRCount;
    int rotLRCount;
    int rotRLCount;

    // Helper to get node height safely
    int getNodeHeight(AVLNode* node) const {
        return node ? node->height : 0;
    }

    // Helper to compute balance factor: height(left) - height(right)
    int getBalanceFactor(AVLNode* node) const {
        return node ? (getNodeHeight(node->left) - getNodeHeight(node->right)) : 0;
    }

    // Right Rotation (LL Rotation)
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = 1 + std::max(getNodeHeight(y->left), getNodeHeight(y->right));
        x->height = 1 + std::max(getNodeHeight(x->left), getNodeHeight(x->right));

        return x; // New root of subtree
    }

    // Left Rotation (RR Rotation)
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = 1 + std::max(getNodeHeight(x->left), getNodeHeight(x->right));
        y->height = 1 + std::max(getNodeHeight(y->left), getNodeHeight(y->right));

        return y; // New root of subtree
    }

    // Recursive insertion with AVL rebalancing
    AVLNode* insertHelper(AVLNode* node, const Book& book) {
        // 1. Standard BST insertion
        if (!node) {
            nodeCount++;
            return new AVLNode(book);
        }

        if (book.bookId < node->data.bookId) {
            node->left = insertHelper(node->left, book);
        } else if (book.bookId > node->data.bookId) {
            node->right = insertHelper(node->right, book);
        } else {
            // Duplicate keys not permitted
            return node;
        }

        // 2. Update height of current ancestor node
        node->height = 1 + std::max(getNodeHeight(node->left), getNodeHeight(node->right));

        // 3. Obtain balance factor to check whether this node became unbalanced
        int balance = getBalanceFactor(node);

        // Case 1: Left-Left (LL) - Right rotation
        if (balance > 1 && book.bookId < node->left->data.bookId) {
            rotLLCount++;
            return rightRotate(node);
        }

        // Case 2: Right-Right (RR) - Left rotation
        if (balance < -1 && book.bookId > node->right->data.bookId) {
            rotRRCount++;
            return leftRotate(node);
        }

        // Case 3: Left-Right (LR) - Double rotation (Left then Right)
        if (balance > 1 && book.bookId > node->left->data.bookId) {
            rotLRCount++;
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Case 4: Right-Left (RL) - Double rotation (Right then Left)
        if (balance < -1 && book.bookId < node->right->data.bookId) {
            rotRLCount++;
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Recursive search helper with comparison counting
    bool searchHelper(AVLNode* node, int bookId, Book& result, int& comparisons) const {
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

    // Recursive in-order traversal
    void inOrderHelper(AVLNode* node, const std::function<void(const Book&)>& visit) const {
        if (!node) return;
        inOrderHelper(node->left, visit);
        visit(node->data);
        inOrderHelper(node->right, visit);
    }

    // Recursive deallocation
    void destroyTree(AVLNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    AVL()
        : root(nullptr), nodeCount(0),
          rotLLCount(0), rotRRCount(0), rotLRCount(0), rotRLCount(0) {}

    ~AVL() {
        destroyTree(root);
        root = nullptr;
        nodeCount = 0;
    }

    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

    /**
     * @brief Inserts a book into the AVL tree, balancing via rotations if necessary.
     */
    void insert(const Book& book) {
        root = insertHelper(root, book);
    }

    /**
     * @brief Searches for a book by BookID, counting the node comparisons.
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
     * @brief Returns the maximum height of the AVL tree.
     */
    int getHeight() const {
        return getNodeHeight(root);
    }

    int getNodeCount() const { return nodeCount; }
    int getLLRotations() const { return rotLLCount; }
    int getRRRotations() const { return rotRRCount; }
    int getLRRotations() const { return rotLRCount; }
    int getRLRotations() const { return rotRLCount; }
    int getTotalRotations() const { return rotLLCount + rotRRCount + rotLRCount + rotRLCount; }
};

#endif // AVL_H
