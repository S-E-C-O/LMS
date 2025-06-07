//
// Created by BlackCyan on 25-6-4.
//

#pragma once
#include <vector>
#include <fstream>
#include <map>
#include <optional>
#include <qstring.h>

enum class Group {
    User,
    Admin
};


class User {
    static constexpr size_t MAX_NAME_SIZE = 50;
    static constexpr size_t MAX_PASSWORD_SIZE = 50;

    char name[MAX_NAME_SIZE]{};
    char password[MAX_PASSWORD_SIZE]{};
    long long id{};
    Group group{};
    std::vector<QString> borrowedBooks;
    std::map<QString, time_t> borrowedTime;

public:
    User() = default;
    User(std::string_view name, std::string_view password, long id, Group group = Group::User);

    // 核心功能
    [[nodiscard]] long getId() const;
    [[nodiscard]] const char* getName() const;
    [[nodiscard]] const char* getPassword() const;
    [[nodiscard]] Group getGroup() const;
    [[nodiscard]] std::vector<QString> getBorrowedBooks() const;

    std::optional<std::time_t> getBorrowTime(const QString& ISBN) const;

    [[nodiscard]] bool checkPassword(std::string_view input) const;
    bool changePassword(std::string_view oldPwd, std::string_view newPwd);
    void resetPassword();

    bool borrowBook(const QString &ISBN);
    bool returnBook(const QString &ISBN);

    // 数据持久化
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

    void setName(const std::string & name);
    void setPassword(const std::string & password);

    void setGroup(Group group);
};
