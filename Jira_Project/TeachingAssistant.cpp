#include "TeachingAssistant.h"

TeachingAssistant::TeachingAssistant(const std::string& user, const std::string& passHash)
    : User(user, passHash, Role::TeachingAssistant) {
}

bool TeachingAssistant::canExecute(CommandType cmd) const {
    return isTACommand(cmd);
}