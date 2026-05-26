#pragma once
#include <vector>
#include <memory>
#include <string>
#include "User.h"
#include "AuthenticationService.h"
#include "CommandParser.h"

class System {
private:
    std::vector<std::shared_ptr<User>> users;
    AuthenticationService authService;
    bool isRunning;

    void initializeAdmin();

public:
    System();
    void run();
    void executeCommand(const Command& cmd);
    
    // Command Handlers
    void handleLogin(const std::vector<std::string>& args);
    void handleLogout();
    void handleHelp();
    void handleViewProfile();
};
