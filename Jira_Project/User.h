#pragma once
#include <string>
#include "Enums.h"

class User {
protected:
    int id;
    static int nextId;
    std::string username;
    std::string passwordHash;
    Role role;

    static bool isBasicCommand(CommandType cmd);
    static bool isStudentCommand(CommandType cmd);
    static bool isTACommand(CommandType cmd);
    static bool isLecturerCommand(CommandType cmd);

public:
    User(const std::string& user, const std::string& passHash, Role r);
    virtual ~User() = default;

    int getId() const;
    std::string getUsername() const;
    std::string getPasswordHash() const;
    Role getRole() const;

    virtual bool canExecute(CommandType cmd) const = 0;
};