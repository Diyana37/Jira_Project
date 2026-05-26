#include "System.h"
#include <iostream>  
#include <print>    
#include <stdexcept>
#include "Administrator.h"

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
    if (cmd.type == CommandType::Unknown) {
        throw std::invalid_argument("Unknown command.");
    }

    if (cmd.type == CommandType::Close) {
        isRunning = false;
        std::println("System closed.");
        return;
    }

    if (cmd.type == CommandType::Login) {
        handleLogin(cmd.args);
        return;
    }

    if (!authService.isLoggedIn()) {
        throw std::invalid_argument("You must be logged in to execute this command.");
    }

    auto currentUser = authService.getCurrentUser();

    if (!currentUser->canExecute(cmd.type)) {
        throw std::invalid_argument("You do not have permission to execute this command.");
    }

    switch (cmd.type) {
    case CommandType::Logout:
        handleLogout();
        break;
    case CommandType::ViewProfile:
        handleViewProfile();
        break;
    case CommandType::Help:
        handleHelp();
        break;
    default:
        std::println("Command not yet implemented in System.");
        break;
    }
}

void System::handleLogin(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        throw std::invalid_argument("Usage: login <username> <password>");
    }

    if (authService.isLoggedIn()) {
        throw std::invalid_argument("You are already logged in. Please logout first.");
    }

    if (authService.login(args[0], args[1], users)) {
        std::println("Welcome, {}!", args[0]);
    }
    else {
        throw std::invalid_argument("Invalid username or password.");
    }
}

void System::handleLogout() {
    if (!authService.isLoggedIn()) {
        throw std::invalid_argument("No user is currently logged in.");
    }
    std::println("Goodbye, {}!", authService.getCurrentUser()->getUsername());
    authService.logout();
}

void System::handleHelp() {
    std::println("Available commands for your role:");
    std::println("- logout: Terminates the active session.");
    std::println("- view-profile: Displays your credentials and role.");
    std::println("- help: Lists commands available to your role.");
    std::println("- close: Safely shuts down the application.");

    auto user = authService.getCurrentUser();
    if (user->getRole() == Role::Administrator) {
        std::println("- create-project <name>");
        std::println("- archive-project <name>");
        std::println("- remove-project <name>");
        std::println("- add-user-to-project <user> <project>");
        std::println("- save");
        std::println("- load");
    }
    if (user->getRole() == Role::Student || user->getRole() == Role::TeachingAssistant || user->getRole() == Role::Lecturer) {
        std::println("- join-project <project_name>");
        std::println("- list-projects");
        std::println("- create-task <project> <type> <priority>");
        std::println("- assign-task <task_id>");
        std::println("- change-status <task_id> <status>");
        std::println("- add-comment <task_id>");
        std::println("- add-tag <task_id> <tag>");
        std::println("- list-tasks");
        std::println("- my-tasks");
        std::println("- upcoming-tasks");
        std::println("- search-tasks <keyword>");
        std::println("- filter-tasks <criteria>");
    }
    if (user->getRole() == Role::TeachingAssistant || user->getRole() == Role::Lecturer) {
        std::println("- review-task <task_id>");
        std::println("- approve-task <task_id>");
        std::println("- stage-report");
        std::println("- start-stage <stage_name>");
        std::println("- finish-stage <stage_name>");
        std::println("- move-task-to-stage <task_id> <stage_name>");
    }
    if (user->getRole() == Role::Lecturer) {
        std::println("- list-all-projects");
        std::println("- list-all-tasks");
        std::println("- finalize-project <project_name>");
        std::println("- grade-task <task_id> <grade>");
        std::println("- student-report <student_name>");
    }
}

void System::handleViewProfile() {
    auto user = authService.getCurrentUser();
    std::println("Username: {}", user->getUsername());

    std::string roleStr;
    switch (user->getRole()) {
    case Role::Student: roleStr = "Student"; break;
    case Role::TeachingAssistant: roleStr = "Teaching Assistant"; break;
    case Role::Lecturer: roleStr = "Lecturer"; break;
    case Role::Administrator: roleStr = "Administrator"; break;
    }
    std::println("Role: {}", roleStr);
}