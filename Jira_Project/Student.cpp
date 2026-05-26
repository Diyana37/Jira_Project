#include "Student.h"

Student::Student(const std::string& user, const std::string& passHash)
    : User(user, passHash, Role::Student) {
}

bool Student::canExecute(CommandType cmd) const {
    return isStudentCommand(cmd);
}