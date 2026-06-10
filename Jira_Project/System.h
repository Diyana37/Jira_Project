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
    AuthenticationService authService;
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

    void handleCreateTask(const std::vector<std::string>& args);
    void handleAssignTask(const std::vector<std::string>& args);
    void handleChangeStatus(const std::vector<std::string>& args);
    void handleAddComment(const std::vector<std::string>& args);
    void handleAddTag(const std::vector<std::string>& args);
    void handleReviewTask(const std::vector<std::string>& args);
    void handleApproveTask(const std::vector<std::string>& args);
    void handleGradeTask(const std::vector<std::string>& args);
    void handleListTasks();
    void handleMyTasks();
    void handleUpcomingTasks();
    void handleSearchTasks(const std::vector<std::string>& args);
    void handleFilterTasks(const std::vector<std::string>& args);
    void handleListAllTasks();
    void handleStudentReport(const std::vector<std::string>& args);

    void handleRemoveUser(const std::vector<std::string>& args);
    void handleRegister(const std::vector<std::string>& args);

    void handleStartStage(const std::vector<std::string>& args);
    void handleFinishStage(const std::vector<std::string>& args);
    void handleMoveTaskToStage(const std::vector<std::string>& args);
    void handleStageReport();

    void handleSave();
    void handleLoad();
};