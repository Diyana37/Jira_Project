#pragma once
#include "User.h"

class Student : public User {
public:
    Student(const std::string& user, const std::string& passHash);

    bool canExecute(CommandType cmd) const override;
};