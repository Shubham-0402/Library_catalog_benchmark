#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Book.h"
#include <vector>
#include <iomanip>
#include <iostream>

/**
 * @enum SlotStatus
 * @brief State of each bucket in the open-addressed hash table.
 */
enum SlotStatus {
    EMPTY,
    OCCUPIED,
    DELETED
};

/**
 * @struct HashEntry
 * @brief Holds a book record and the bucket status for linear probing.
 */
struct HashEntry {
    Book data;
    SlotStatus status;

    HashEntry() : data(), status(EMPTY) {}
};

/**
 * @class HashTable
 * @brief Open-addressed Hash Table using the Division Method (mod 53) and Linear Probing.
 */
class HashTable {
public:
    static const int TABLE_SIZE = 53; // Fixed prime table size as mandated

private:
    std::vector<HashEntry> table;
    int itemCount;
    int totalCollisions;

public:
    HashTable() : table(TABLE_SIZE), itemCount(0), totalCollisions(0) {}

    /**
     * @brief Division hash function: h(k) = k mod 53
     */
    int hashFunction(int key) const {
        return ((key % TABLE_SIZE) + TABLE_SIZE) % TABLE_SIZE;
    }

    /**
     * @brief Inserts a book into the hash table using Linear Probing.
     * @param book The Book record to insert
     * @param collisions Output parameter recording the collisions encountered for this specific insertion
     * @return true if successfully inserted, false if table is full or duplicate exists
     */
    bool insert(const Book& book, int& collisions) {
        collisions = 0;
        if (itemCount >= TABLE_SIZE) {
            return false; // Table is full
        }

        int initialHash = hashFunction(book.bookId);
        int firstDeletedIndex = -1;

        for (int i = 0; i < TABLE_SIZE; i++) {
            int probeIndex = (initialHash + i) % TABLE_SIZE;

            if (table[probeIndex].status == EMPTY) {
                // Determine target index (reuse deleted slot if one was previously encountered)
                int targetIndex = (firstDeletedIndex != -1) ? firstDeletedIndex : probeIndex;
                table[targetIndex].data = book;
                table[targetIndex].status = OCCUPIED;
                itemCount++;
                totalCollisions += collisions;
                return true;
            } else if (table[probeIndex].status == DELETED) {
                if (firstDeletedIndex == -1) {
                    firstDeletedIndex = probeIndex;
                }
                collisions++;
            } else { // OCCUPIED
                if (table[probeIndex].data.bookId == book.bookId) {
                    return false; // Duplicate key
                }
                collisions++;
            }
        }

        // If all slots checked and we found a deleted slot
        if (firstDeletedIndex != -1) {
            table[firstDeletedIndex].data = book;
            table[firstDeletedIndex].status = OCCUPIED;
            itemCount++;
            totalCollisions += collisions;
            return true;
        }

        return false;
    }

    /**
     * @brief Convenience overload for insertion when individual collision counter is not required.
     */
    bool insert(const Book& book) {
        int temp = 0;
        return insert(book, temp);
    }

    /**
     * @brief Searches for a book by BookID, counting probes/comparisons.
     * @param bookId The ID to look up
     * @param result Output parameter to receive the found book
     * @param probes Output parameter recording number of probe comparisons made
     * @return true if found, false otherwise
     */
    bool search(int bookId, Book& result, int& probes) const {
        probes = 0;
        int initialHash = hashFunction(bookId);

        for (int i = 0; i < TABLE_SIZE; i++) {
            probes++;
            int probeIndex = (initialHash + i) % TABLE_SIZE;

            if (table[probeIndex].status == EMPTY) {
                // Linear probing stops at an empty slot
                return false;
            }

            if (table[probeIndex].status == OCCUPIED && table[probeIndex].data.bookId == bookId) {
                result = table[probeIndex].data;
                return true;
            }
        }

        return false; // Table searched completely without match
    }

    /**
     * @brief Returns total accumulated collisions during all insertions.
     */
    int getTotalCollisions() const {
        return totalCollisions;
    }

    /**
     * @brief Returns current number of occupied records.
     */
    int getItemCount() const {
        return itemCount;
    }

    /**
     * @brief Returns current load factor (alpha = n / m).
     */
    double getLoadFactor() const {
        return static_cast<double>(itemCount) / static_cast<double>(TABLE_SIZE);
    }

    /**
     * @brief Displays the internal hash table bucket allocation for viva inspection.
     */
    void displayTable() const {
        std::cout << "\n+-------+--------+--------+------------------------------------------------------+\n";
        std::cout << "| Slot  | Status | BookID | Title                                                |\n";
        std::cout << "+-------+--------+--------+------------------------------------------------------+\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            std::cout << "| [" << std::setw(2) << i << "] | ";
            if (table[i].status == OCCUPIED) {
                std::cout << "OCCUP  | " << std::setw(6) << table[i].data.bookId
                          << " | " << std::left << std::setw(52)
                          << (table[i].data.title.length() > 52 ? table[i].data.title.substr(0, 49) + "..." : table[i].data.title)
                          << std::right << " |\n";
            } else if (table[i].status == DELETED) {
                std::cout << "DELETE | ------ | ---------------------------------------------------- |\n";
            } else {
                std::cout << "EMPTY  | ------ | ---------------------------------------------------- |\n";
            }
        }
        std::cout << "+-------+--------+--------+------------------------------------------------------+\n";
    }
};

#endif // HASHTABLE_H
