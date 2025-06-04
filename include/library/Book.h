//
// Created by BlackCyan on 25-6-4.
//

#pragma once
#include <string_view>

class Book {
    static constexpr size_t MAX_SIZE = 50;

    char title[MAX_SIZE]{};
    char author[MAX_SIZE]{};
    char publisher[MAX_SIZE]{};
    long publishYear{};
    long ISBN{};

public:
    explicit Book(std::string_view title, std::string_view author, std::string_view publisher, long publishYear, long ISBN);

    ~Book() = default;

    // 拷贝构造函数
    Book(const Book&) = default;
    Book& operator=(const Book&) = default;

    // 移动构造函数
    Book(Book&&) = default;
    Book& operator=(Book&&) = default;

    [[nodiscard]] const char* getTitle() const;
    [[nodiscard]] const char* getAuthor() const;
    [[nodiscard]] const char* getPublisher() const;
    [[nodiscard]] long getPublishYear() const;
    [[nodiscard]] long getISBN() const;
};
