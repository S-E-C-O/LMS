//
// Created by BlackCyan on 25-6-4.
//

#include "User.h"
#include <stdexcept>
#include <algorithm>
#include <fstream>

User::User(const std::string_view name,
           const long id,
           const std::string_view password,
           const Group group)
    : id(id), group(group) {

    auto copyString = [](const std::string_view& src, char* dest, const size_t maxSize) {
        if (src.size() >= maxSize) {
            throw std::invalid_argument("String too long");
        }
        std::ranges::copy(src, dest);
        dest[src.size()] = '\0';
    };

    copyString(name, this->name, MAX_SIZE);
    copyString(password, this->password, MAX_SIZE);
}

void User::serialize(std::ofstream& out) const {
    out.write(name, MAX_SIZE);
    out.write(reinterpret_cast<const char*>(&id), sizeof(id));
    out.write(password, MAX_SIZE);
    out.write(reinterpret_cast<const char*>(&group), sizeof(group));

    const size_t count = borrowedBooks.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (size_t i = 0; i < count; ++i) {
        out.write(reinterpret_cast<const char*>(&borrowedBooks[i]), sizeof(long));
        out.write(reinterpret_cast<const char*>(&borrowDates[i]), sizeof(time_t));
    }
}

void User::deserialize(std::ifstream& in) {
    in.read(name, MAX_SIZE);
    in.read(reinterpret_cast<char*>(&id), sizeof(id));
    in.read(password, MAX_SIZE);
    in.read(reinterpret_cast<char*>(&group), sizeof(group));

    size_t count;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    borrowedBooks.resize(count);
    borrowDates.resize(count);
    for (size_t i = 0; i < count; ++i) {
        in.read(reinterpret_cast<char*>(&borrowedBooks[i]), sizeof(long));
        in.read(reinterpret_cast<char*>(&borrowDates[i]), sizeof(time_t));
    }
}

const char* User::getName() const { return name; }
long User::getId() const { return id; }
User::Group User::getGroup() const { return group; }
const std::vector<long>& User::getBorrowedBooks() const { return borrowedBooks; }
bool User::isAdmin() const { return group == Group::ADMIN; }

bool User::verifyPassword(const std::string_view password) const {
    return std::string_view(this->password) == password;
}

void User::changePassword(std::string_view newPassword) {
    if (newPassword.size() >= MAX_SIZE) {
        throw std::invalid_argument("New password too long");
    }
    std::ranges::copy(newPassword, password);
    password[newPassword.size()] = '\0';
}

void User::addBorrowedBook(const long ISBN) {
    borrowedBooks.push_back(ISBN);
    borrowDates.push_back(time(nullptr)); // 添加当前时间
}

void User::removeBorrowedBook(long ISBN) {
    auto it = std::ranges::find(borrowedBooks, ISBN);
    if (it != borrowedBooks.end()) {
        borrowedBooks.erase(it);
        auto dateIt = borrowDates.begin() + (it - borrowedBooks.begin());
        borrowDates.erase(dateIt);
    }
}
