//
// Created by BlackCyan on 25-6-4.
//

#include "User.h"
#include <cstring>
#include <stdexcept>
#include <optional>

#include <QRegularExpression>

User::User(std::string_view name, std::string_view password, long id, Group group)
    : id(id), group(group) {
    if (name.size() >= MAX_NAME_SIZE || password.size() >= MAX_PASSWORD_SIZE) {
        throw std::invalid_argument("Name or password too long");
    }

    std::memcpy(this->name, name.data(), name.size());
    this->name[name.size()] = '\0';

    std::memcpy(this->password, password.data(), password.size());
    this->password[password.size()] = '\0';
}

long User::getId() const { return id; }
const char* User::getName() const { return name; }
const char* User::getPassword() const { return password; }
Group User::getGroup() const { return group; }
std::vector<QString> User::getBorrowedBooks() const { return borrowedBooks; }
std::optional<std::time_t> User::getBorrowTime(const QString& ISBN) const {
    if (borrowedTime.contains(ISBN)) {
        return borrowedTime.at(ISBN);
    }
    return std::nullopt;
}

void User::setName(const std::string & name) {
    if (name.size() >= MAX_NAME_SIZE) {
        throw std::invalid_argument("Name too long");
    }

    std::memcpy(this->name, name.data(), name.size());
    this->name[name.size()] = '\0';
}
void User::setPassword(const std::string & password) {
    if (password.size() >= MAX_PASSWORD_SIZE) {
        throw std::invalid_argument("Password too long");
    }

    std::memcpy(this->password, password.data(), password.size());
    this->password[password.size()] = '\0';
}
void User::setGroup(const Group group) {
    this->group = group;
}

bool User::checkPassword(std::string_view input) const {
    return std::string_view(password) == input;
}

bool User::changePassword(const std::string_view oldPwd, const std::string_view newPwd) {
    if (checkPassword(oldPwd)) {
        if (newPwd.size() >= MAX_PASSWORD_SIZE) {
            throw std::invalid_argument("Password too long");
        }
        std::memcpy(this->password, newPwd.data(), newPwd.size());
        this->password[newPwd.size()] = '\0';
        return true;
    }
    return false;
}

void User::resetPassword() {
    setPassword("123456");
}

bool User::borrowBook(const QString& ISBN) {
    if (borrowedBooks.size() >= 5) return false;
    if (std::ranges::find(borrowedBooks, ISBN) != borrowedBooks.end()) return false;
    borrowedBooks.push_back(ISBN);
    borrowedTime[ISBN] = std::time(nullptr);
    return true;
}

bool User::returnBook(const QString& ISBN) {
    if (const auto it = std::ranges::find(borrowedBooks, ISBN); it != borrowedBooks.end()) {
        borrowedBooks.erase(it);
        borrowedTime.erase(ISBN);
        return true;
    }
    return false;
}

void User::serialize(QDataStream& out) const {
    out.writeRawData(name, MAX_NAME_SIZE);
    out.writeRawData(password, MAX_PASSWORD_SIZE);
    out << static_cast<quint64>(id);
    out << static_cast<qint32>(group);

    out << static_cast<quint32>(borrowedBooks.size());
    for (const auto& isbn : borrowedBooks) {
        out << isbn;
        std::time_t timeVal = borrowedTime.contains(isbn) ? borrowedTime.at(isbn) : 0;
        out << timeVal;
    }
}

void User::deserialize(QDataStream& in) {
    if (in.readRawData(name, MAX_NAME_SIZE) != MAX_NAME_SIZE) throw std::runtime_error("Failed to read name");
    if (in.readRawData(password, MAX_PASSWORD_SIZE) != MAX_PASSWORD_SIZE) throw std::runtime_error("Failed to read password");

    in >> id;

    qint32 groupInt;
    in >> groupInt;
    group = static_cast<Group>(groupInt);

    quint32 borrowedCount;
    in >> borrowedCount;

    borrowedBooks.clear();
    borrowedTime.clear();
    for (quint32 i = 0; i < borrowedCount; ++i) {
        QString isbn;
        in >> isbn;

        qint64 timeVal;
        in >> timeVal;

        borrowedBooks.push_back(isbn);
        borrowedTime[isbn] = static_cast<std::time_t>(timeVal);
    }
}
