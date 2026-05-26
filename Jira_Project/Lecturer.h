#pragma once
#include "User.h"

class Lecturer : public User {
public:
    Lecturer(const std::string& user, const std::string& passHash);

    bool canExecute(CommandType cmd) const override;
};