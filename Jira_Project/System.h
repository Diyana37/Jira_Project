#pragma once
#include <vector>
#include <memory>
#include <string>
#include "User.h"
#include "Project.h" 
#include "AuthenticationService.h"
#include "CommandParser.h"

class System {
private:
    std::vector<std::shared_ptr<User>> users;
    std::vector<std::shared_ptr<Project>> projects; 
    bool isRunning;

    void initializeAdmin();

public:
    System();
    void run();
    void executeCommand(const Command& cmd);

    void handleLogin(const std::vector<std::string>& args);
    void handleLogout();
    void handleHelp();
    void handleViewProfile();

    void handleCreateProject(const std::vector<std::string>& args);
    void handleArchiveProject(const std::vector<std::string>& args);
    void handleRemoveProject(const std::vector<std::string>& args);
    void handleFinalizeProject(const std::vector<std::string>& args);
    void handleAddUserToProject(const std::vector<std::string>& args);
    void handleJoinProject(const std::vector<std::string>& args);
    void handleListProjects();
    void handleListAllProjects();
};