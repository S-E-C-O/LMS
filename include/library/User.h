//
// Created by BlackCyan on 25-6-4.
//

#pragma once
#include <string_view>
#include <vector>
#include <fstream>

enum class Group {
    Reader,
    Admin
};

class User {
    static constexpr size_t MAX_NAME_SIZE = 50;
    static constexpr size_t MAX_PASSWORD_SIZE = 50;

    char name[MAX_NAME_SIZE]{};
    char password[MAX_PASSWORD_SIZE]{};
    long id{};
    Group group{};
    std::vector<long> borrowedBooks;  // 存储借阅图书的 ISBN

public:
    User() = default;
    User(std::string_view name, std::string_view password, long id, Group group = Group::Reader);

    // 核心功能
    [[nodiscard]] long getId() const;
    [[nodiscard]] const char* getName() const;
    [[nodiscard]] const char* getPassword() const;
    [[nodiscard]] Group getGroup() const;
    [[nodiscard]] const std::vector<long>& getBorrowedBooks() const;

    [[nodiscard]] bool checkPassword(std::string_view input) const;
    bool changePassword(std::string_view oldPwd, std::string_view newPwd);
    void resetPassword();

    bool borrowBook(long ISBN);
    bool returnBook(long ISBN);

    // 数据持久化
    void serialize(std::ofstream& out) const;
    void deserialize(std::ifstream& in);

    void setName(const std::string & name);
    void setPassword(const std::string & password);
};
