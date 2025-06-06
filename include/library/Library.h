//
// Created by BlackCyan on 25-6-6.
//

#pragma once

#include "User.h"
#include "Book.h"
#include <vector>
#include <filesystem>

class Library {
    std::vector<User> users;
    std::vector<Book> books;
    std::filesystem::path user_data_file_path;
    std::filesystem::path book_data_file_path;


public:
    void setDataFilePaths(const std::filesystem::path& userPath, const std::filesystem::path& bookPath);

    // 用户操作
    bool registerUser(const User& user);
    bool deleteUser(long id);
    bool updateUser(const User& updatedUser);
    User* loginUser(long id, std::string_view password);
    User* findUserById(long id);
    [[nodiscard]] const std::vector<User>& getAllUsers() const;

    // 图书操作
    bool addBook(const Book& book);
    bool deleteBook(long ISBN);
    bool updateBook(const Book& updatedBook);
    Book* findBookByISBN(long ISBN);
    [[nodiscard]] const std::vector<Book>& getAllBooks() const;

    // 图书搜索
    [[nodiscard]] std::vector<Book> searchBooksByTitle(std::string_view keyword) const;
    [[nodiscard]] std::vector<Book> searchBooksByAuthor(std::string_view keyword) const;
    [[nodiscard]] std::vector<Book> searchBooksByPublisher(std::string_view keyword) const;

    bool borrowBook(long userId, long ISBN);
    bool returnBook(long userId, long ISBN);

    // 数据持久化
    void saveToFile(const std::filesystem::path &userFile, const std::filesystem::path &bookFile) const;
    void loadFromFile(const std::filesystem::path &userFile, const std::filesystem::path &bookFile);
};
