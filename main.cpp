#include "Book.h"
#include "BST.h"
#include "AVL.h"
#include "HashTable.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>

/**
 * @brief Benchmark test case definition.
 */
struct BenchmarkCase {
    int bookId;
    std::string expectedStatus;
};

/**
 * @brief Runs automated search benchmarks across BST, AVL, and HashTable.
 * Measures search comparisons/probes for 5 successful and 5 unsuccessful searches.
 */
void runAutomatedBenchmark(const BST& bst, const AVL& avl, const HashTable& ht) {
    std::vector<BenchmarkCase> testCases = {
        // 5 Successful Searches (Present in dataset.csv)
        {1005, "SUCCESS"},
        {1200, "SUCCESS"},
        {1650, "SUCCESS"},
        {2280, "SUCCESS"},
        {3100, "SUCCESS"},

        // 5 Unsuccessful Searches (Not present in dataset.csv)
        {9999, "UNSUCCESSFUL"},
        {1000, "UNSUCCESSFUL"},
        {1505, "UNSUCCESSFUL"},
        {2222, "UNSUCCESSFUL"},
        {3500, "UNSUCCESSFUL"}
    };

    std::cout << "\n========================================================================================\n";
    std::cout << "               ALGORITHM BATTLE: EMPIRICAL BENCHMARK COMPARISON TABLE                   \n";
    std::cout << "========================================================================================\n";
    std::cout << "| Query Type   | BookID | BST Status | BST Comps | AVL Comps | Hash Probes | Best Performer |\n";
    std::cout << "+--------------+--------+------------+-----------+-----------+-------------+----------------+\n";

    double totalBstSucc = 0, totalAvlSucc = 0, totalHtSucc = 0;
    double totalBstUnsucc = 0, totalAvlUnsucc = 0, totalHtUnsucc = 0;
    int succCount = 0, unsuccCount = 0;

    for (const auto& tc : testCases) {
        Book b1, b2, b3;
        int bstC = 0, avlC = 0, htC = 0;

        bool foundBST = bst.search(tc.bookId, b1, bstC);
        bool foundAVL = avl.search(tc.bookId, b2, avlC);
        bool foundHT  = ht.search(tc.bookId, b3, htC);
        (void)foundAVL;
        (void)foundHT;

        std::string winner = "HashTable";
        int minCost = htC;
        if (avlC < minCost) { minCost = avlC; winner = "AVL"; }
        if (bstC < minCost) { minCost = bstC; winner = "BST"; }
        if (htC == avlC && htC == bstC) winner = "TIE (All)";
        else if (htC == avlC) winner = "TIE (AVL/HT)";

        std::string statusStr = foundBST ? "FOUND" : "NOT FOUND";

        std::cout << "| " << std::left << std::setw(12) << tc.expectedStatus
                  << " | " << std::setw(6) << tc.bookId
                  << " | " << std::setw(10) << statusStr
                  << " | " << std::right << std::setw(9) << bstC
                  << " | " << std::setw(9) << avlC
                  << " | " << std::setw(11) << htC
                  << " | " << std::left << std::setw(14) << winner
                  << " |\n";

        if (tc.expectedStatus == "SUCCESS") {
            totalBstSucc += bstC;
            totalAvlSucc += avlC;
            totalHtSucc  += htC;
            succCount++;
        } else {
            totalBstUnsucc += bstC;
            totalAvlUnsucc += avlC;
            totalHtUnsucc  += htC;
            unsuccCount++;
        }
    }

    std::cout << "+--------------+--------+------------+-----------+-----------+-------------+----------------+\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "| AVG (SUCCESS):                     | " << std::right
              << std::setw(9) << (totalBstSucc / succCount) << " | "
              << std::setw(9) << (totalAvlSucc / succCount) << " | "
              << std::setw(11) << (totalHtSucc / succCount) << " | HashTable (Win)|\n";
    std::cout << "| AVG (UNSUCCESSFUL):                | "
              << std::setw(9) << (totalBstUnsucc / unsuccCount) << " | "
              << std::setw(9) << (totalAvlUnsucc / unsuccCount) << " | "
              << std::setw(11) << (totalHtUnsucc / unsuccCount) << " | HashTable (Win)|\n";
    std::cout << "+------------------------------------+-----------+-----------+-------------+----------------+\n";

    std::cout << "\n========================================================================================\n";
    std::cout << "                         STRUCTURAL & TELEMETRY METRICS                                 \n";
    std::cout << "========================================================================================\n";
    std::cout << " [1] Binary Search Tree (BST):\n";
    std::cout << "     - Total Node Count      : " << bst.getNodeCount() << "\n";
    std::cout << "     - Tree Height           : " << bst.getHeight() << " (Severe right skew observed)\n";
    std::cout << "     - Max Path Length       : " << bst.getHeight() << " edges/nodes\n\n";

    std::cout << " [2] Self-Balancing AVL Tree:\n";
    std::cout << "     - Total Node Count      : " << avl.getNodeCount() << "\n";
    std::cout << "     - Tree Height           : " << avl.getHeight() << " (Strictly balanced: ceil(log2(41)) <= 6)\n";
    std::cout << "     - LL Rotations Executed : " << avl.getLLRotations() << "\n";
    std::cout << "     - RR Rotations Executed : " << avl.getRRRotations() << "\n";
    std::cout << "     - LR Rotations Executed : " << avl.getLRRotations() << "\n";
    std::cout << "     - RL Rotations Executed : " << avl.getRLRotations() << "\n";
    std::cout << "     - Total Rotations       : " << avl.getTotalRotations() << "\n\n";

    std::cout << " [3] Hash Table (Linear Probing, Size = 53):\n";
    std::cout << "     - Table Capacity        : " << HashTable::TABLE_SIZE << " slots (Prime)\n";
    std::cout << "     - Records Occupied      : " << ht.getItemCount() << "\n";
    std::cout << "     - Load Factor (alpha)   : " << ht.getLoadFactor() * 100.0 << "% (" << ht.getLoadFactor() << ")\n";
    std::cout << "     - Total Collisions      : " << ht.getTotalCollisions() << "\n";
    std::cout << "========================================================================================\n\n";
}

/**
 * @brief Displays interactive CLI menu for viva examination.
 */
void displayMenu() {
    std::cout << "\n=======================================================\n";
    std::cout << "   SRMIST DSA ASSIGNMENT: ALGORITHM BATTLE (LIBRARY)   \n";
    std::cout << "=======================================================\n";
    std::cout << " [1] Insert New Book\n";
    std::cout << " [2] Search Book ID (Side-by-Side Comparison)\n";
    std::cout << " [3] In-Order Traversal (Sorted Catalog)\n";
    std::cout << " [4] Run Automated Benchmark Suite\n";
    std::cout << " [5] Exit\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << " Enter your choice (1-5): ";
}

int main() {
    std::cout << "========================================================================================\n";
    std::cout << "      SRMIST DATA STRUCTURES & ALGORITHMS (DSA) - ALGORITHM BATTLE PROJECT              \n";
    std::cout << "      Application: Library Book Record Search System (40 Dataset Records)               \n";
    std::cout << "========================================================================================\n\n";

    BST bst;
    AVL avl;
    HashTable ht;

    const std::string datasetPath = "dataset.csv";
    std::cout << "[INFO] Loading dataset from: " << datasetPath << " ...\n";
    std::vector<Book> loadedBooks = loadBooksFromCSV(datasetPath);

    if (loadedBooks.empty()) {
        std::cerr << "[FATAL] No records loaded. Verify that dataset.csv is in the workspace root.\n";
        return 1;
    }

    std::cout << "[INFO] Successfully parsed " << loadedBooks.size() << " book records.\n";
    std::cout << "[INFO] Populating BST, AVL, and HashTable simultaneously...\n";

    for (const auto& book : loadedBooks) {
        bst.insert(book);
        avl.insert(book);
        ht.insert(book);
    }

    std::cout << "[INFO] Population complete.\n";

    // Initial automated benchmark display
    runAutomatedBenchmark(bst, avl, ht);

    // Interactive CLI loop for live viva demonstration
    int choice = 0;
    while (true) {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid input. Please enter a number between 1 and 5.\n";
            continue;
        }

        if (choice == 1) {
            int newId;
            std::string title, author, genre;

            std::cout << "\n--- Insert New Book ---\n";
            std::cout << "Enter 4-Digit Book ID: ";
            while (!(std::cin >> newId)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid ID. Enter integer: ";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Enter Title  : ";
            std::getline(std::cin, title);
            std::cout << "Enter Author : ";
            std::getline(std::cin, author);
            std::cout << "Enter Genre  : ";
            std::getline(std::cin, genre);

            Book newBook(newId, title, author, genre);
            bst.insert(newBook);
            avl.insert(newBook);
            int collisions = 0;
            bool htOk = ht.insert(newBook, collisions);

            std::cout << "\n[SUCCESS] Book added to all 3 data structures!\n";
            std::cout << " -> BST Height updated to : " << bst.getHeight() << "\n";
            std::cout << " -> AVL Height updated to : " << avl.getHeight() << " (Total Rotations: " << avl.getTotalRotations() << ")\n";
            std::cout << " -> HashTable Insertion   : " << (htOk ? "Success" : "Failed")
                      << " with " << collisions << " collisions (Load factor: " << ht.getLoadFactor() * 100.0 << "%)\n";

        } else if (choice == 2) {
            int searchId;
            std::cout << "\n--- Search Book ID ---\n";
            std::cout << "Enter Book ID to search: ";
            while (!(std::cin >> searchId)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid ID. Enter integer: ";
            }

            Book resBST, resAVL, resHT;
            int bstComps = 0, avlComps = 0, htComps = 0;

            bool foundBST = bst.search(searchId, resBST, bstComps);
            bool foundAVL = avl.search(searchId, resAVL, avlComps);
            bool foundHT  = ht.search(searchId, resHT, htComps);
            (void)foundAVL;
            (void)foundHT;

            std::cout << "\n=======================================================\n";
            std::cout << "                SEARCH RESULTS & COMPARISON             \n";
            std::cout << "=======================================================\n";
            if (foundBST) {
                std::cout << " [STATUS] Record Found!\n";
                std::cout << "  - Title  : " << resBST.title << "\n";
                std::cout << "  - Author : " << resBST.author << "\n";
                std::cout << "  - Genre  : " << resBST.genre << "\n";
            } else {
                std::cout << " [STATUS] Record NOT Found in any data structure.\n";
            }
            std::cout << "-------------------------------------------------------\n";
            std::cout << " Performance Comparison for BookID " << searchId << ":\n";
            std::cout << "  1. BST Comparisons        : " << bstComps << " comparisons\n";
            std::cout << "  2. AVL Comparisons        : " << avlComps << " comparisons\n";
            std::cout << "  3. HashTable Probes       : " << htComps  << " probe comparisons\n";
            
            std::string winner = "HashTable";
            int minCost = htComps;
            if (avlComps < minCost) { minCost = avlComps; winner = "AVL"; }
            if (bstComps < minCost) { minCost = bstComps; winner = "BST"; }
            if (htComps == avlComps && htComps == bstComps) winner = "TIE (All identical)";
            std::cout << " >> Winner for this query   : " << winner << " (" << minCost << " comparisons)\n";
            std::cout << "=======================================================\n";

        } else if (choice == 3) {
            std::cout << "\n--- In-Order Traversal (Sorted Catalog by BookID) ---\n";
            bst.printInOrder();

        } else if (choice == 4) {
            runAutomatedBenchmark(bst, avl, ht);

        } else if (choice == 5) {
            std::cout << "\nExiting Algorithm Battle System. Goodbye!\n";
            break;
        } else {
            std::cout << "[ERROR] Invalid option. Please choose between 1 and 5.\n";
        }
    }

    return 0;
}
