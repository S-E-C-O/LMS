//
// Created by BlackCyan on 25-6-4.
//

#pragma once
#include <string_view>
#include <fstream>

class Book {
    static constexpr size_t MAX_SIZE = 50;

    char title[MAX_SIZE]{};
    char author[MAX_SIZE]{};
    char publisher[MAX_SIZE]{};
    int publishYear{};
    long ISBN{};
    int availableCopies{};
    int totalCopies{};

public:
    explicit Book(std::string_view title = "",
                  std::string_view author = "",
                  std::string_view publisher = "",
                  int publishYear = 0,
                  long ISBN = 0,
                  int totalCopies = 1);

    void serialize(std::ofstream& out) const;
    void deserialize(std::ifstream& in);

    [[nodiscard]] const char* getTitle() const;
    [[nodiscard]] const char* getAuthor() const;
    [[nodiscard]] const char* getPublisher() const;
    [[nodiscard]] int getPublishYear() const;
    [[nodiscard]] long getISBN() const;
    [[nodiscard]] int getAvailableCopies() const;
    [[nodiscard]] int getTotalCopies() const;

    void setTitle(std::string_view title);
    void setAuthor(std::string_view author);

    void decreaseAvailableCopies();
    void increaseAvailableCopies();
};
