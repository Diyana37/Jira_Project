#include "Lecturer.h"

Lecturer::Lecturer(const std::string& user, const std::string& passHash)
    : User(user, passHash, Role::Lecturer) {
}

bool Lecturer::canExecute(CommandType cmd) const {
    return isLecturerCommand(cmd);
}