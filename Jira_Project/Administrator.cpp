#include "Administrator.h"

Administrator::Administrator(const std::string& user, const std::string& passHash)
    : User(user, passHash, Role::Administrator) {
}

bool Administrator::canExecute(CommandType cmd) const {
    return isBasicCommand(cmd) ||
        cmd == CommandType::Register ||
        cmd == CommandType::RemoveUser ||
        cmd == CommandType::CreateProject ||
        cmd == CommandType::ArchiveProject ||
        cmd == CommandType::RemoveProject ||
        cmd == CommandType::AddUserToProject ||
        cmd == CommandType::ListProjects ||     
        cmd == CommandType::ListAllProjects ||   
        cmd == CommandType::ListAllTasks;     
}