#pragma once
#include "User.h"

class TeachingAssistant : public User {
public:
    TeachingAssistant(const std::string& user, const std::string& passHash);

    bool canExecute(CommandType cmd) const override;
};