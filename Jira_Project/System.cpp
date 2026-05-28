#include "System.h"
#include "Administrator.h"
#include <iostream>  
#include <print>  
#include <stdexcept>
#include <algorithm>

System::System() : isRunning(true) {
    initializeAdmin();
}

void System::initializeAdmin() {
    users.push_back(std::make_shared<Administrator>("admin", AuthenticationService::hashPassword("admin")));
}

void System::run() {
    std::string line;
    std::println("Welcome to FMI JIRA System");

    while (isRunning) {
        std::print("> ");
        if (!std::getline(std::cin, line)) {
            break;
        }

        try {
            Command cmd = CommandParser::parse(line);
            executeCommand(cmd);
        }
        catch (const std::exception& e) {
            std::println("Error: {}", e.what());
        }
    }
}

void System::executeCommand(const Command& cmd) {
    if (cmd.type == CommandType::Unknown) throw std::invalid_argument("Unknown command.");

    if (cmd.type == CommandType::Close) {
        isRunning = false;
        std::println("System closed.");
        return;
    }

    if (cmd.type == CommandType::Login) {
        handleLogin(cmd.args);
        return;
    }

    if (!authService.isLoggedIn()) throw std::invalid_argument("You must be logged in to execute this command.");

    auto currentUser = authService.getCurrentUser();

    if (!currentUser->canExecute(cmd.type)) {
        throw std::invalid_argument("You do not have permission to execute this command.");
    }

    switch (cmd.type) {
    case CommandType::Logout: handleLogout(); break;
    case CommandType::ViewProfile: handleViewProfile(); break;
    case CommandType::Help: handleHelp(); break;

    case CommandType::CreateProject: handleCreateProject(cmd.args); break;
    case CommandType::ArchiveProject: handleArchiveProject(cmd.args); break;
    case CommandType::RemoveProject: handleRemoveProject(cmd.args); break;
    case CommandType::FinalizeProject: handleFinalizeProject(cmd.args); break;
    case CommandType::AddUserToProject: handleAddUserToProject(cmd.args); break;
    case CommandType::JoinProject: handleJoinProject(cmd.args); break;
    case CommandType::ListProjects: handleListProjects(); break;
    case CommandType::ListAllProjects: handleListAllProjects(); break;

    default:
        std::println("Command not yet implemented in System.");
        break;
    }
}

void System::handleLogin(const std::vector<std::string>& args) {
    if (args.size() != 2) throw std::invalid_argument("Usage: login <username> <password>");
    if (authService.isLoggedIn()) throw std::invalid_argument("You are already logged in. Please logout first.");

    if (authService.login(args[0], args[1], users)) {
        std::println("Welcome, {}!", args[0]);
    }
    else {
        throw std::invalid_argument("Invalid username or password.");
    }
}

void System::handleLogout() {
    if (!authService.isLoggedIn()) throw std::invalid_argument("No user is currently logged in.");
    std::println("Goodbye, {}!", authService.getCurrentUser()->getUsername());
    authService.logout();
}

void System::handleHelp() {
    std::println("Use basic commands: login, logout, help, close.");
    std::println("Project commands: create-project, join-project, list-all-projects, etc.");
}

void System::handleViewProfile() {
    auto user = authService.getCurrentUser();
    std::println("Username: {}", user->getUsername());
}

void System::handleCreateProject(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: create-project <name>");

    for (const auto& proj : projects) {
        if (proj->getName() == args[0]) throw std::invalid_argument("Error: Project already exists.");
    }
    projects.push_back(std::make_shared<Project>(args[0]));
    std::println("Project '{}' created successfully.", args[0]);
}

void System::handleArchiveProject(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: archive-project <name>");

    for (const auto& proj : projects) {
        if (proj->getName() == args[0]) {
            proj->setArchived(true);
            std::println("Project '{}' archived.", args[0]);
            return;
        }
    }
    throw std::invalid_argument("Error: Project not found.");
}

void System::handleRemoveProject(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: remove-project <name>");

    for (auto it = projects.begin(); it != projects.end(); ++it) {
        if ((*it)->getName() == args[0]) {
            projects.erase(it);
            std::println("Project '{}' removed.", args[0]);
            return;
        }
    }
    throw std::invalid_argument("Error: Project not found.");
}

void System::handleFinalizeProject(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: finalize-project <name>");

    for (const auto& proj : projects) {
        if (proj->getName() == args[0]) {
            proj->setFinalized(true);
            std::println("Project '{}' finalized.", args[0]);
            return;
        }
    }
    throw std::invalid_argument("Error: Project not found.");
}

void System::handleAddUserToProject(const std::vector<std::string>& args) {
    if (args.size() != 2) throw std::invalid_argument("Usage: add-user-to-project <user> <project>");

    std::shared_ptr<User> targetUser = nullptr;
    for (const auto& u : users) {
        if (u->getUsername() == args[0]) { targetUser = u; break; }
    }
    if (!targetUser) throw std::invalid_argument("Error: User not found.");

    for (const auto& p : projects) {
        if (p->getName() == args[1]) {
            p->addMember(targetUser);
            std::println("User {} added to project {}.", args[0], args[1]);
            return;
        }
    }
    throw std::invalid_argument("Error: Project not found.");
}

void System::handleJoinProject(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: join-project <project_name>");

    for (const auto& p : projects) {
        if (p->getName() == args[0]) {
            p->addMember(authService.getCurrentUser());
            std::println("Successfully joined project {}.", args[0]);
            return;
        }
    }
    throw std::invalid_argument("Error: Project not found.");
}

void System::handleListProjects() {
    auto currentUser = authService.getCurrentUser()->getUsername();
    std::println("Your Projects:");
    bool found = false;
    for (const auto& p : projects) {
        if (p->hasMember(currentUser)) {
            std::println("- {} (Archived: {}, Finalized: {})", p->getName(), p->getIsArchived(), p->getIsFinalized());
            found = true;
        }
    }
    if (!found) std::println("You are not a member of any projects.");
}

void System::handleListAllProjects() {
    std::println("All System Projects:");
    if (projects.empty()) {
        std::println("No projects found.");
        return;
    }
    for (const auto& p : projects) {
        std::println("- {} (Archived: {}, Finalized: {})", p->getName(), p->getIsArchived(), p->getIsFinalized());
    }
}