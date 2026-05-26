#include "User.h"

int User::nextId = 1;

User::User(const std::string& user, const std::string& passHash, Role r)
    : username(user), passwordHash(passHash), role(r), id(nextId++) 
{
}

int User::getId() const {
    return id;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getPasswordHash() const {
    return passwordHash;
}

Role User::getRole() const {
    return role;
}

bool User::isBasicCommand(CommandType cmd) {
    return cmd == CommandType::Login ||
        cmd == CommandType::Logout ||
        cmd == CommandType::ViewProfile ||
        cmd == CommandType::Help ||
        cmd == CommandType::Close;
}

bool User::isStudentCommand(CommandType cmd) {
    return isBasicCommand(cmd) ||
        cmd == CommandType::JoinProject ||
        cmd == CommandType::ListProjects ||
        cmd == CommandType::CreateTask ||
        cmd == CommandType::AssignTask ||
        cmd == CommandType::ChangeStatus ||
        cmd == CommandType::AddComment ||
        cmd == CommandType::AddTag ||
        cmd == CommandType::ListTasks ||
        cmd == CommandType::MyTasks ||
        cmd == CommandType::UpcomingTasks ||
        cmd == CommandType::SearchTasks ||
        cmd == CommandType::FilterTasks;
}

bool User::isTACommand(CommandType cmd) {
    return isStudentCommand(cmd) ||
        cmd == CommandType::ReviewTask ||
        cmd == CommandType::ApproveTask ||
        cmd == CommandType::StageReport ||
        cmd == CommandType::StartStage ||
        cmd == CommandType::FinishStage ||
        cmd == CommandType::MoveTaskToStage;
}

bool User::isLecturerCommand(CommandType cmd) {
    return isTACommand(cmd) ||
        cmd == CommandType::FinalizeProject ||
        cmd == CommandType::ListAllProjects ||
        cmd == CommandType::ListAllTasks ||
        cmd == CommandType::GradeTask ||
        cmd == CommandType::StudentReport;
}