//
// Created by BlackCyan on 25-6-6.
//

#pragma once

#include "User.h"
#include "Book.h"
#include <vector>
#include <filesystem>
#include <qstring.h>

class Library {
    std::vector<User> users;
    std::vector<Book> books;
    std::filesystem::path user_data_file_path;
    std::filesystem::path book_data_file_path;


public:
    void setDataFilePaths(const std::filesystem::path& userPath, const std::filesystem::path& bookPath);
    std::vector<std::filesystem::path> getDataFilePaths();

    // 用户操作
    bool registerUser(const User& user);
    bool deleteUser(long id);
    bool updateUser(const User& updatedUser);
    User* loginUser(long id, std::string_view password);
    User* findUserById(long id);
    [[nodiscard]] const std::vector<User>& getAllUsers() const;

    // 图书操作
    bool addBook(const Book& book);
    bool deleteBook(const QString& ISBN);
    bool updateBook(const Book& updatedBook);
    Book* findBookByISBN(const QString& ISBN);
    [[nodiscard]] const std::vector<Book>& getAllBooks() const;

    std::vector<Book> searchBooksByISBN(std::string_view keyword) const;

    // 图书搜索
    [[nodiscard]] std::vector<Book> searchBooksByTitle(std::string_view keyword) const;
    [[nodiscard]] std::vector<Book> searchBooksByAuthor(std::string_view keyword) const;
    [[nodiscard]] std::vector<Book> searchBooksByPublisher(std::string_view keyword) const;

    bool borrowBook(long userId, const QString& ISBN);
    bool returnBook(long userId, const QString& ISBN);

    // 数据持久化
    void saveToFile(const std::filesystem::path &userFile, const std::filesystem::path &bookFile) const;
    void loadFromFile(const std::filesystem::path &userFile, const std::filesystem::path &bookFile);
};
