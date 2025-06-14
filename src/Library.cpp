//
// Created by BlackCyan on 25-6-6.
//

#include "Library.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <algorithm>
#include <stdexcept>

#include "CompressionUtil.h"

void Library::setDataFilePaths(const std::filesystem::path& userPath, const std::filesystem::path& bookPath) {
    user_data_file_path = userPath;
    book_data_file_path = bookPath;
}

std::vector<std::filesystem::path> Library::getDataFilePaths() {
    return {user_data_file_path, book_data_file_path};
}

bool Library::registerUser(const User& user) {
    if (std::ranges::any_of(users,
                            [&user](const User& u) { return u.getId() == user.getId(); })) {
        return false;
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

bool Library::addBook(const Book& book) {
    if (!book.isValidISBN()) {
        qWarning() << "无效ISBN，添加失败！";
        return false;
    }
    if (std::ranges::any_of(books,
        [&book](const Book& b) { return b.getISBN() == book.getISBN(); })) {
        return false;
    }
    books.push_back(book);
    saveToFile(user_data_file_path, book_data_file_path);
    return true;
}

bool Library::deleteBook(const QString& ISBN) {
    auto it = std::ranges::remove_if(books,
        [&ISBN](const Book& b) { return b.getISBN() == ISBN; }).begin();
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

Book* Library::findBookByISBN(const QString& ISBN) {
    for (auto& book : books) {
        if (book.getISBN() == ISBN)
            return &book;
    }
    return nullptr;
}

const std::vector<Book>& Library::getAllBooks() const {
    return books;
}

std::vector<Book> Library::searchBooksByISBN(const std::string_view keyword) const {
    std::vector<Book> results;
    for (const auto& book : books) {
        if (std::string(book.getISBN().toStdString()).find(keyword) != std::string::npos) {
            results.push_back(book);
        }
    }
    return results;
}


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

bool Library::borrowBook(long userId, const QString& ISBN) {
    User* user = findUserById(userId);
    Book* book = findBookByISBN(ISBN);
    if (!user || !book || book->getAvailableCopies() <= 0)
        return false;
    if (!user->borrowBook(ISBN))
        return false;
    book->decreaseAvailableCopies();
    saveToFile(user_data_file_path, book_data_file_path);
    return true;
}

bool Library::returnBook(long userId, const QString& ISBN) {
    User* user = findUserById(userId);
    Book* book = findBookByISBN(ISBN);
    if (!user || !book)
        return false;
    if (!user->returnBook(ISBN))
        return false;
    book->increaseAvailableCopies();
    saveToFile(user_data_file_path, book_data_file_path);
    return true;
}
bool Library::isBookBorrowedByUser(int userId, const std::string& isbn)  {
    auto user = findUserById(userId);
    if (!user) return false;
    const auto& borrowedBooks = user->getBorrowedBooks(); // 假设返回 vector<string> 或 set<string>
    return std::find(borrowedBooks.begin(), borrowedBooks.end(), isbn) != borrowedBooks.end();
}


void Library::saveToFile(const std::filesystem::path &userFile, const std::filesystem::path &bookFile) const {
    QFile userQFile(QString::fromStdString(userFile.string()));
    QFile bookQFile(QString::fromStdString(bookFile.string()));

    if (!userQFile.open(QIODevice::WriteOnly) || !bookQFile.open(QIODevice::WriteOnly))
        throw std::runtime_error("Failed to open data files for saving");

    QDataStream uout(&userQFile);
    QDataStream bout(&bookQFile);

    uout << static_cast<quint32>(users.size());
    for (const auto& user : users) {
        QByteArray compressed = CompressionUtil::compressObject([&](QDataStream& s) {
            user.serialize(s);
        });
        uout << compressed;
    }

    bout << static_cast<quint32>(books.size());
    for (const auto& book : books) {
        QByteArray compressed = CompressionUtil::compressObject([&](QDataStream& s) {
            book.serialize(s);
        });
        bout << compressed;
    }

    userQFile.close();
    bookQFile.close();
}


void Library::loadFromFile(const std::filesystem::path &userFile, const std::filesystem::path &bookFile) {
    users.clear();
    books.clear();

    QFile userQFile(QString::fromStdString(userFile.string()));
    if (!userQFile.exists()) {
        User adminUser("Admin", "P@ssw0rd", 100, Group::Admin);
        users.push_back(adminUser);
        saveToFile(userFile, bookFile);
        return;
    }

    if (userQFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&userQFile);
        quint32 userCount;
        in >> userCount;

        for (quint32 i = 0; i < userCount; ++i) {
            QByteArray compressed;
            in >> compressed;

            User temp;
            CompressionUtil::decompressObject(compressed, [&](QDataStream& s) {
                temp.deserialize(s);
            });

            users.push_back(temp);
        }
        userQFile.close();
    }

    QFile bookQFile(QString::fromStdString(bookFile.string()));
    if (bookQFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&bookQFile);
        quint32 bookCount;
        in >> bookCount;

        for (quint32 i = 0; i < bookCount; ++i) {
            QByteArray compressed;
            in >> compressed;

            Book temp;
            CompressionUtil::decompressObject(compressed, [&](QDataStream& s) {
                temp.deserialize(s);
            });

            books.push_back(temp);
        }
        bookQFile.close();
    }
}

std::vector<Book> Library::getBooksBorrowedByUser(int userId) {
    std::vector<Book> result;
    auto user = findUserById(userId);
    if (!user) return result;

    const auto& borrowedIsbns = user->getBorrowedBooks();
    for (const auto& isbn : borrowedIsbns) {
        auto book = findBookByISBN(isbn);
        if (book) result.push_back(*book);
    }
    return result;
}
