//
// Created by BlackCyan on 25-6-4.
//

#include "User.h"

#include <stdexcept>

User::User(std::string_view name, const long id, std::string_view password) : id(id) {

    // 检查并复制 name
    if (name.size() >= MAX_SIZE) {
        throw std::invalid_argument("name too long");
    }
    std::ranges::copy(name, this->name);
    this->name[name.size()] = '\0';

    // 检查并复制 password
    if (password.size() >= MAX_SIZE) {
        throw std::invalid_argument("password too long");
    }
    std::ranges::copy(password, this->password);
    this->password[password.size()] = '\0';
}

const char* User::getName() const {
    return this->name;
}

long User::getId() const {
    return this->id;
}
