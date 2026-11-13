#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

/**
 * @struct Book
 * @brief Represents a single Library Book record in the Library Management System.
 */
struct Book {
    int bookId;
    std::string title;
    std::string author;
    std::string genre;

    // Default constructor
    Book() : bookId(0), title(""), author(""), genre("") {}

    // Parameterized constructor
    Book(int id, const std::string& t, const std::string& a, const std::string& g)
        : bookId(id), title(t), author(a), genre(g) {}

    // Display helper
    void display() const {
        std::cout << "[ID: " << std::setw(4) << bookId << "] "
                  << "\"" << title << "\" by " << author
                  << " (" << genre << ")\n";
    }
};

/**
 * @brief Utility function to trim whitespace and carriage returns from string ends.
 */
inline std::string trimString(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Loads library book records from a CSV file.
 * Expected format: BookID,Title,Author,Genre
 * 
 * @param filename Relative or absolute path to dataset.csv
 * @return std::vector<Book> List of loaded book objects
 */
inline std::vector<Book> loadBooksFromCSV(const std::string& filename) {
    std::vector<Book> books;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "[Error] Failed to open CSV file: " << filename << std::endl;
        return books;
    }

    std::string line;
    bool isFirstLine = true;

    while (std::getline(file, line)) {
        std::string trimmedLine = trimString(line);
        if (trimmedLine.empty()) continue;

        // Skip CSV header line
        if (isFirstLine) {
            isFirstLine = false;
            if (trimmedLine.rfind("BookID", 0) == 0) {
                continue;
            }
        }

        std::stringstream ss(trimmedLine);
        std::string idStr, title, author, genre;

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, title, ',') &&
            std::getline(ss, author, ',') &&
            std::getline(ss, genre)) {
            
            try {
                int id = std::stoi(trimString(idStr));
                books.emplace_back(id, trimString(title), trimString(author), trimString(genre));
            } catch (const std::exception& e) {
                std::cerr << "[Warning] Skipping malformed row: " << trimmedLine << " (" << e.what() << ")\n";
            }
        }
    }

    file.close();
    return books;
}

#endif // BOOK_H
