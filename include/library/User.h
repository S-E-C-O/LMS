//
// Created by BlackCyan on 25-6-4.
//

#pragma once
#include <string_view>

class User {
public:
    enum class Group {
        NORMAL,
        ADMIN
    };

private:
    static constexpr size_t MAX_SIZE = 50;

    char name[MAX_SIZE]{};
    long id{};
    char password[MAX_SIZE]{};
    Group group{};

public:
    explicit User(std::string_view name, long id, std::string_view password);

    ~User() = default;

    [[nodiscard]] const char* getName() const;
    [[nodiscard]] long getId() const;
};
