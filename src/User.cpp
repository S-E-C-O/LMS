//
// Created by BlackCyan on 25-6-4.
//

#include "User.h"
#include <cstring>
#include <stdexcept>
#include <algorithm>

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
const std::vector<long>& User::getBorrowedBooks() const { return borrowedBooks; }

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

bool User::borrowBook(long ISBN) {
    if (std::find(borrowedBooks.begin(), borrowedBooks.end(), ISBN) != borrowedBooks.end())
        return false;
    borrowedBooks.push_back(ISBN);
    return true;
}

bool User::returnBook(long ISBN) {
    auto it = std::find(borrowedBooks.begin(), borrowedBooks.end(), ISBN);
    if (it != borrowedBooks.end()) {
        borrowedBooks.erase(it);
        return true;
    }
    return false;
}

void User::serialize(std::ofstream& out) const {
    out.write(name, MAX_NAME_SIZE);
    out.write(password, MAX_PASSWORD_SIZE);
    out.write(reinterpret_cast<const char*>(&id), sizeof(id));
    out.write(reinterpret_cast<const char*>(&group), sizeof(group));

    size_t count = borrowedBooks.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (long isbn : borrowedBooks) {
        out.write(reinterpret_cast<const char*>(&isbn), sizeof(isbn));
    }
}

void User::deserialize(std::ifstream& in) {
    if (!in.read(name, MAX_NAME_SIZE)) throw std::runtime_error("Failed to read name");
    if (!in.read(password, MAX_PASSWORD_SIZE)) throw std::runtime_error("Failed to read password");
    if (!in.read(reinterpret_cast<char*>(&id), sizeof(id))) throw std::runtime_error("Failed to read ID");
    if (!in.read(reinterpret_cast<char*>(&group), sizeof(group))) throw std::runtime_error("Failed to read group");

    size_t count = 0;
    if (!in.read(reinterpret_cast<char*>(&count), sizeof(count))) throw std::runtime_error("Failed to read book count");
    borrowedBooks.clear();
    for (size_t i = 0; i < count; ++i) {
        long isbn = 0;
        if (!in.read(reinterpret_cast<char*>(&isbn), sizeof(isbn))) throw std::runtime_error("Failed to read isbn");
        borrowedBooks.push_back(isbn);
    }
}

