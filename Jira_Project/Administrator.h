#pragma once
#include "User.h"

class Administrator : public User {
public:
    Administrator(const std::string& user, const std::string& passHash);

    bool canExecute(CommandType cmd) const override;
};