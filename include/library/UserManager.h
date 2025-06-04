//
// Created by BlackCyan on 25-6-4.
//

#pragma once
#include "User.h"
#include <vector>
#include <unordered_map>
#include <optional>
#include <filesystem>

class UserManager {
    static constexpr char FILENAME[] = "users.dat";
    static constexpr char SALT[] = "S0m3S4ltV4lu3";

    std::unordered_map<long, User> users;
    std::filesystem::path filePath;

    [[nodiscard]] std::string hashPassword(std::string_view password) const;
    bool loadFromFile();
    bool saveToFile();

public:
    explicit UserManager(const std::filesystem::path& path = FILENAME);

    bool addUser(std::string_view name, long id, std::string_view password,
                 User::Group group = User::Group::NORMAL);
    bool deleteUser(long id);
    bool updateUser(long id, std::optional<std::string_view> name = std::nullopt,
                    std::optional<std::string_view> password = std::nullopt,
                    std::optional<User::Group> group = std::nullopt);
    [[nodiscard]] std::optional<User> findUser(long id) const;
    [[nodiscard]] bool authenticate(long id, std::string_view password) const;
    [[nodiscard]] std::vector<User> getAllUsers() const;
};
