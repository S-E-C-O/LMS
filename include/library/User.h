//
// Created by BlackCyan on 25-6-4.
//

#pragma once
#include <string_view>
#include <vector>
#include <ctime>
#include <memory>

class Library; // 前向声明

class User {
public:
    enum class Group { NORMAL, ADMIN };

private:
    static constexpr size_t MAX_SIZE = 50;
    char name[MAX_SIZE]{};
    long id{};
    char password[MAX_SIZE]{};
    Group group{};
    std::vector<long> borrowedBooks;
    std::vector<time_t> borrowDates;

public:
    explicit User(std::string_view name = "",
                  long id = 0,
                  std::string_view password = "",
                  Group group = Group::NORMAL);

    void serialize(std::ofstream& out) const;
    void deserialize(std::ifstream& in);

    [[nodiscard]] const char* getName() const;
    [[nodiscard]] long getId() const;
    [[nodiscard]] Group getGroup() const;
    [[nodiscard]] const std::vector<long>& getBorrowedBooks() const;
    [[nodiscard]] bool isAdmin() const;

    friend class Library;
    void addBorrowedBook(long ISBN);
    void removeBorrowedBook(long ISBN);

    [[nodiscard]] bool verifyPassword(std::string_view password) const;
    void changePassword(std::string_view newPassword);
};
