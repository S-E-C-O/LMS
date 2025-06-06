//
// Created by BlackCyan on 25-6-6.
//

#include "Library.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdexcept>

void Library::setDataFilePaths(const std::filesystem::path& userPath, const std::filesystem::path& bookPath) {
    user_data_file_path = userPath;
    book_data_file_path = bookPath;
}


// 用户操作
bool Library::registerUser(const User& user) {
    if (std::ranges::any_of(users,
                            [&user](const User& u) { return u.getId() == user.getId(); })) {
        return false;  // 重复 ID 不允许
    }
    users.push_back(user);
    saveToFile(user_data_file_path, book_data_file_path);
    return true;
}

bool Library::deleteUser(long id) {
    const auto it = std::ranges::remove_if(users,
                                     [id](const User& u) { return u.getId() == id; }).begin();
    if (it != users.end()) {
        users.erase(it, users.end());
        saveToFile(user_data_file_path, book_data_file_path);
        return true;
    }
    return false;
}

bool Library::updateUser(const User& updatedUser) {
    for (auto& user : users) {
        if (user.getId() == updatedUser.getId()) {
            user = updatedUser;
            saveToFile(user_data_file_path, book_data_file_path);
            return true;
        }
    }
    return false;
}

User* Library::loginUser(const long id, const std::string_view password) {
    for (auto& user : users) {
        if (user.getId() == id && user.checkPassword(password))
            return &user;
    }
    return nullptr;
}

User* Library::findUserById(long id) {
    for (auto& user : users) {
        if (user.getId() == id)
            return &user;
    }
    return nullptr;
}

const std::vector<User>& Library::getAllUsers() const {
    return users;
}

// 图书操作
bool Library::addBook(const Book& book) {
    if (std::ranges::any_of(books,
                            [&book](const Book& b) { return b.getISBN() == book.getISBN(); })) {
        return false;  // 重复 ISBN 不允许
    }
    books.push_back(book);
    saveToFile(user_data_file_path, book_data_file_path);
    return true;
}

bool Library::deleteBook(long ISBN) {
    const auto it = std::ranges::remove_if(books,
                                     [ISBN](const Book& b) { return b.getISBN() == ISBN; }).begin();
    if (it != books.end()) {
        books.erase(it, books.end());
        saveToFile(user_data_file_path, book_data_file_path);
        return true;
    }
    return false;
}

bool Library::updateBook(const Book& updatedBook) {
    for (auto& book : books) {
        if (book.getISBN() == updatedBook.getISBN()) {
            book = updatedBook;
            saveToFile(user_data_file_path, book_data_file_path);
            return true;
        }
    }
    return false;
}

Book* Library::findBookByISBN(long ISBN) {
    for (auto& book : books) {
        if (book.getISBN() == ISBN)
            return &book;
    }
    return nullptr;
}

const std::vector<Book>& Library::getAllBooks() const {
    return books;
}

// 图书搜索
std::vector<Book> Library::searchBooksByTitle(const std::string_view keyword) const {
    if (keyword.empty()) return books;
    std::vector<Book> results;
    for (const auto& book : books) {
        if (std::string(book.getTitle()).find(keyword) != std::string::npos)
            results.push_back(book);
    }
    return results;
}

std::vector<Book> Library::searchBooksByAuthor(const std::string_view keyword) const {
    std::vector<Book> results;
    for (const auto& book : books) {
        if (std::string(book.getAuthor()).find(keyword) != std::string::npos)
            results.push_back(book);
    }
    return results;
}

std::vector<Book> Library::searchBooksByPublisher(const std::string_view keyword) const {
    std::vector<Book> results;
    for (const auto& book : books) {
        if (std::string(book.getPublisher()).find(keyword) != std::string::npos)
            results.push_back(book);
    }
    return results;
}

// 借书
bool Library::borrowBook(long userId, long ISBN) {
    User* user = findUserById(userId);
    Book* book = findBookByISBN(ISBN);
    if (!user || !book || book->getAvailableCopies() <= 0)
        return false;
    if (!user->borrowBook(ISBN))
        return false;
    book->decreaseAvailableCopies();
    return true;
}

// 还书
bool Library::returnBook(long userId, long ISBN) {
    User* user = findUserById(userId);
    Book* book = findBookByISBN(ISBN);
    if (!user || !book)
        return false;
    if (!user->returnBook(ISBN))
        return false;
    book->increaseAvailableCopies();
    return true;
}

// 数据持久化
void Library::saveToFile(const std::filesystem::path &userFile, const std::filesystem::path &bookFile) const {
    std::ofstream uout(userFile, std::ios::binary);
    std::ofstream bout(bookFile, std::ios::binary);
    if (!uout || !bout)
        throw std::runtime_error("Failed to open data files for saving");

    size_t userCount = users.size();
    uout.write(reinterpret_cast<const char*>(&userCount), sizeof(userCount));
    for (const auto& user : users)
        user.serialize(uout);

    size_t bookCount = books.size();
    bout.write(reinterpret_cast<const char*>(&bookCount), sizeof(bookCount));
    for (const auto& book : books)
        book.serialize(bout);
}

void Library::loadFromFile(const std::filesystem::path &userFile, const std::filesystem::path &bookFile) {
    users.clear();
    if (!exists(userFile)) {
        // 创建默认管理员用户
        User adminUser("Admin", "P@ssw0rd", 100, Group::Admin);
        users.push_back(adminUser);

        std::ofstream ofs(userFile, std::ios::binary);
        if (!ofs) {
            std::cerr << "Failed to create user data file\n";
            return;
        }

        // 写入用户数量 + serialize 格式
        size_t userCount = users.size();
        ofs.write(reinterpret_cast<const char*>(&userCount), sizeof(userCount));
        for (const auto& u : users) {
            u.serialize(ofs);
        }

        ofs.close();
        std::cout << "Created default admin user and saved using structured format.\n";
    } else {
        // 读取用户数量 + deserialize 格式
        std::ifstream ifs(userFile, std::ios::binary);
        if (!ifs) {
            std::cerr << "Failed to open user data file\n";
            return;
        }

        size_t userCount = 0;
        ifs.read(reinterpret_cast<char*>(&userCount), sizeof(userCount));
        for (size_t i = 0; i < userCount; ++i) {
            User temp;
            temp.deserialize(ifs);
            users.push_back(temp);
        }

        ifs.close();
    }

    // 处理书籍
    books.clear();
    if (exists(bookFile)) {
        std::ifstream ifs(bookFile, std::ios::binary);
        if (!ifs) {
            std::cerr << "Failed to open book data file\n";
            return;
        }

        size_t bookCount = 0;
        ifs.read(reinterpret_cast<char*>(&bookCount), sizeof(bookCount));
        for (size_t i = 0; i < bookCount; ++i) {
            Book temp;
            temp.deserialize(ifs);
            books.push_back(temp);
        }

        ifs.close();
    }
}

