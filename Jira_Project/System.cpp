#include "System.h"
#include "Administrator.h"
#include "Task.h"
#include "Student.h"
#include "TeachingAssistant.h"
#include "Lecturer.h"
#include "Stage.h"
#include <iostream>  
#include <print>  
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "CommandParser.h"

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

    if (cmd.type == CommandType::Load) {
        handleLoad();
        return;
    }

    if (cmd.type == CommandType::Save) {
        handleSave();
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
    case CommandType::Register: handleRegister(cmd.args); break;

    case CommandType::CreateProject: handleCreateProject(cmd.args); break;
    case CommandType::ArchiveProject: handleArchiveProject(cmd.args); break;
    case CommandType::RemoveProject: handleRemoveProject(cmd.args); break;
    case CommandType::FinalizeProject: handleFinalizeProject(cmd.args); break;
    case CommandType::AddUserToProject: handleAddUserToProject(cmd.args); break;
    case CommandType::JoinProject: handleJoinProject(cmd.args); break;
    case CommandType::ListProjects: handleListProjects(); break;
    case CommandType::ListAllProjects: handleListAllProjects(); break;

    case CommandType::CreateTask: handleCreateTask(cmd.args); break;
    case CommandType::AssignTask: handleAssignTask(cmd.args); break;
    case CommandType::ChangeStatus: handleChangeStatus(cmd.args); break;
    case CommandType::AddComment: handleAddComment(cmd.args); break;
    case CommandType::AddTag: handleAddTag(cmd.args); break;
    case CommandType::ReviewTask: handleReviewTask(cmd.args); break;
    case CommandType::ApproveTask: handleApproveTask(cmd.args); break;
    case CommandType::GradeTask: handleGradeTask(cmd.args); break;

    case CommandType::ListTasks: handleListTasks(); break;
    case CommandType::MyTasks: handleMyTasks(); break;
    case CommandType::UpcomingTasks: handleUpcomingTasks(); break;
    case CommandType::SearchTasks: handleSearchTasks(cmd.args); break;
    case CommandType::FilterTasks: handleFilterTasks(cmd.args); break;
    case CommandType::ListAllTasks: handleListAllTasks(); break;
    case CommandType::StudentReport: handleStudentReport(cmd.args); break;
    case CommandType::RemoveUser: handleRemoveUser(cmd.args); break;

    case CommandType::StartStage: handleStartStage(cmd.args); break;
    case CommandType::FinishStage: handleFinishStage(cmd.args); break;
    case CommandType::MoveTaskToStage: handleMoveTaskToStage(cmd.args); break;
    case CommandType::StageReport: handleStageReport(); break;

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
        handleHelp();
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
    std::println("=== Available Commands ===");
    std::println("- Basic: login, logout, help, close, save, load, view-profile");

    if (!authService.isLoggedIn()) return;

    auto role = authService.getCurrentUser()->getRole();

    if (role == Role::Administrator) {
        std::println("- Admin: register, remove-user, create-project, archive-project, remove-project, add-user-to-project");
    }
    else if (role == Role::Lecturer) {
        std::println("- Lecturer: finalize-project, list-all-projects, list-all-tasks, grade-task, student-report");
        std::println("  (Inherits TA and Student commands):");
        std::println("  - TA: approve-task, review-task, start-stage, finish-stage, stage-report, move-task-to-stage");
        std::println("  - Student: list-projects, join-project, create-task, assign-task, change-status, add-comment, add-tag, my-tasks, list-tasks, upcoming-tasks, search-tasks, filter-tasks");
    }
    else if (role == Role::TeachingAssistant) {
        std::println("- TA: approve-task, review-task, start-stage, finish-stage, stage-report, move-task-to-stage");
        std::println("  (Inherits Student commands):");
        std::println("  - Student: list-projects, join-project, create-task, assign-task, change-status, add-comment, add-tag, my-tasks, list-tasks, upcoming-tasks, search-tasks, filter-tasks");
    }
    else if (role == Role::Student) {
        std::println("- Student: list-projects, join-project, create-task, assign-task, change-status, add-comment, add-tag, my-tasks, list-tasks, upcoming-tasks, search-tasks, filter-tasks");
    }
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

    std::string desc;
    std::print("Description: ");
    std::getline(std::cin, desc);

    projects.push_back(std::make_shared<Project>(args[0], desc));
    projects.back()->addMember(authService.getCurrentUser());

    std::println("[System] Project '{}' created successfully.", args[0]);
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

TaskType parseTaskType(const std::string& str) {
    if (str == "Feature") return TaskType::Feature;
    if (str == "Improvement") return TaskType::Improvement;
    if (str == "Bug") return TaskType::Bug;
    return TaskType::Task;
}

Priority parsePriority(const std::string& str) {
    if (str == "Low") return Priority::Low;
    if (str == "High") return Priority::High;
    if (str == "Critical") return Priority::Critical;
    return Priority::Medium;
}

Status parseStatus(const std::string& str) {
    if (str == "InProgress") return Status::InProgress;
    if (str == "InReview") return Status::InReview;
    if (str == "Done") return Status::Done;
    return Status::ToDo;
}

void System::handleCreateTask(const std::vector<std::string>& args) {
    if (args.size() != 3) throw std::invalid_argument("Usage: create-task <project> <type> <priority>");

    std::shared_ptr<Project> targetProj = nullptr;
    for (const auto& p : projects) {
        if (p->getName() == args[0]) { targetProj = p; break; }
    }
    if (!targetProj) throw std::invalid_argument("Error: Project not found.");

    TaskType type = parseTaskType(args[1]);
    Priority prio = parsePriority(args[2]);

    std::string title, desc, deadline;
    int points = 0;

    std::print("Title: ");
    std::getline(std::cin, title);

    std::print("Description: ");
    std::getline(std::cin, desc);

    std::print("Deadline (e.g. 26-05-2026): ");
    std::getline(std::cin, deadline);

    std::print("Points: ");

    if (!(std::cin >> points)) {
        points = 0;
        std::cin.clear(); 
    }

    std::cin.ignore(10000, '\n');

    auto newTask = std::make_shared<Task>(title, desc, deadline, points, type, prio);
    targetProj->addTask(newTask);

    std::println("[System] Task {} created successfully.", newTask->getId());
}

void System::handleAssignTask(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: assign-task <task_id>");

    std::shared_ptr<Task> targetTask = nullptr;
    for (const auto& p : projects) {
        targetTask = p->getTaskById(args[0]);
        if (targetTask) break;
    }
    if (!targetTask) throw std::invalid_argument("Error: Task not found.");

    auto currentUser = authService.getCurrentUser();
    targetTask->assignUser(currentUser, currentUser->getUsername());
    std::println("Task {} assigned to {}.", args[0], currentUser->getUsername());
}

void System::handleChangeStatus(const std::vector<std::string>& args) {
    if (args.size() != 2) throw std::invalid_argument("Usage: change-status <task_id> <status>");

    std::shared_ptr<Task> targetTask = nullptr;
    for (const auto& p : projects) {
        targetTask = p->getTaskById(args[0]);
        if (targetTask) break;
    }
    if (!targetTask) throw std::invalid_argument("Error: Task not found.");

    Status newStatus = parseStatus(args[1]);
    targetTask->setStatus(newStatus, authService.getCurrentUser()->getUsername());
    std::println("Task {} status updated.", args[0]);
}

void System::handleAddComment(const std::vector<std::string>& args) {
    if (args.size() < 2) throw std::invalid_argument("Usage: add-comment <task_id> <comment text>");

    std::shared_ptr<Task> targetTask = nullptr;
    for (const auto& p : projects) {
        targetTask = p->getTaskById(args[0]);
        if (targetTask) break;
    }
    if (!targetTask) throw std::invalid_argument("Error: Task not found.");

    std::string commentText = "";
    for (size_t i = 1; i < args.size(); ++i) {
        commentText += args[i] + (i == args.size() - 1 ? "" : " ");
    }

    targetTask->addComment(authService.getCurrentUser()->getUsername(), commentText);
    std::println("Comment added to task {}.", args[0]);
}

void System::handleAddTag(const std::vector<std::string>& args) {
    if (args.size() != 2) throw std::invalid_argument("Usage: add-tag <task_id> <tag>");

    std::shared_ptr<Task> targetTask = nullptr;
    for (const auto& p : projects) {
        targetTask = p->getTaskById(args[0]);
        if (targetTask) break;
    }
    if (!targetTask) throw std::invalid_argument("Error: Task not found.");

    targetTask->addTag(args[1]);
    std::println("Tag '{}' added to task {}.", args[1], args[0]);
}

void System::handleReviewTask(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: review-task <task_id>");

    std::shared_ptr<Task> targetTask = nullptr;
    for (const auto& p : projects) {
        targetTask = p->getTaskById(args[0]);
        if (targetTask) break;
    }
    if (!targetTask) throw std::invalid_argument("Error: Task not found.");

    targetTask->setStatus(Status::InReview, authService.getCurrentUser()->getUsername());
    std::println("Task {} moved to InReview.", args[0]);
}

void System::handleApproveTask(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: approve-task <task_id>");

    std::shared_ptr<Task> targetTask = nullptr;
    for (const auto& p : projects) {
        targetTask = p->getTaskById(args[0]);
        if (targetTask) break;
    }
    if (!targetTask) throw std::invalid_argument("Error: Task not found.");

    targetTask->setStatus(Status::Done, authService.getCurrentUser()->getUsername());
    std::println("Task {} approved and moved to Done.", args[0]);
}

void System::handleGradeTask(const std::vector<std::string>& args) {
    if (args.size() != 2) throw std::invalid_argument("Usage: grade-task <task_id> <grade>");

    std::shared_ptr<Task> targetTask = nullptr;
    for (const auto& p : projects) {
        targetTask = p->getTaskById(args[0]);
        if (targetTask) break;
    }
    if (!targetTask) throw std::invalid_argument("Error: Task not found.");

    try {
        int grade = std::stoi(args[1]);
        targetTask->setGrade(grade, authService.getCurrentUser()->getUsername());
        std::println("Task {} graded with a {}.", args[0], grade);
    }
    catch (const std::invalid_argument&) {
        throw std::invalid_argument("Error: Grade must be a valid number.");
    }
}

void System::handleListTasks() {
    std::println("Available Tasks:");
    for (const auto& p : projects) {
        for (const auto& t : p->getTasks()) {
            std::println("- {} | {} | {}", t->getId(), static_cast<int>(t->getStatus()), static_cast<int>(t->getPriority()));
        }
    }
}

void System::handleListAllTasks() {
    std::println("All System Tasks:");
    for (const auto& p : projects) {
        for (const auto& t : p->getTasks()) {
            std::println("- {} | {} | {}", t->getId(), static_cast<int>(t->getStatus()), static_cast<int>(t->getPriority()));
        }
    }
}

void System::handleMyTasks() {
    auto currentUser = authService.getCurrentUser()->getUsername();
    int completed = 0;
    int inProgress = 0;
    int score = 0;

    for (const auto& p : projects) {
        for (const auto& t : p->getTasks()) {
            if (auto assignee = t->getAssignee().lock()) {
                if (assignee->getUsername() == currentUser) {
                    if (t->getStatus() == Status::Done) {
                        completed++;
                        score += (t->getGrade() > 0 ? t->getGrade() : t->getPoints());
                    }
                    else if (t->getStatus() == Status::InProgress) {
                        inProgress++;
                    }
                }
            }
        }
    }
    std::println("Completed tasks: {}", completed);
    std::println("In progress: {}", inProgress);
    std::println("Performance score: {}", score);
}

void System::handleStudentReport(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: student-report <student_name>");

    std::string targetUser = args[0];
    int completed = 0;
    int totalGrade = 0;
    int score = 0;

    for (const auto& p : projects) {
        for (const auto& t : p->getTasks()) {
            if (auto assignee = t->getAssignee().lock()) {
                if (assignee->getUsername() == targetUser && t->getStatus() == Status::Done) {
                    completed++;
                    if (t->getGrade() > 0) totalGrade += t->getGrade();
                    score += (t->getGrade() > 0 ? t->getGrade() : t->getPoints());
                }
            }
        }
    }

    std::println("Completed tasks: {}", completed);
    if (completed > 0 && totalGrade > 0) {
        std::println("Average grade: {}", totalGrade / completed);
    }
    else {
        std::println("Average grade: 0");
    }
    std::println("Performance score: {}", score);
}

void System::handleUpcomingTasks() {
    std::println("Upcoming Tasks:");
    for (const auto& p : projects) {
        for (const auto& t : p->getTasks()) {
            if (t->getStatus() != Status::Done && !t->getDeadline().empty()) {
                std::println("- {} (Due: {})", t->getId(), t->getDeadline());
            }
        }
    }
}

void System::handleSearchTasks(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: search-tasks <keyword>");
    std::string keyword = args[0];

    std::println("Search Results for '{}':", keyword);
    for (const auto& p : projects) {
        for (const auto& t : p->getTasks()) {
            if (t->getTitle().find(keyword) != std::string::npos || t->getDescription().find(keyword) != std::string::npos) {
                std::println("- {}: {}", t->getId(), t->getTitle());
            }
        }
    }
}

void System::handleFilterTasks(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: filter-tasks <criteria>");
    std::string criteria = args[0];

    std::println("Filtered Tasks ({}):", criteria);
    bool foundAny = false;

    for (const auto& p : projects) {
        for (const auto& t : p->getTasks()) {
            std::string statusStr;
            switch (t->getStatus()) {
            case Status::ToDo: statusStr = "ToDo"; break;
            case Status::InProgress: statusStr = "InProgress"; break;
            case Status::InReview: statusStr = "InReview"; break;
            case Status::Done: statusStr = "Done"; break;
            }

            std::string prioStr;
            switch (t->getPriority()) {
            case Priority::Low: prioStr = "Low"; break;
            case Priority::Medium: prioStr = "Medium"; break;
            case Priority::High: prioStr = "High"; break;
            case Priority::Critical: prioStr = "Critical"; break;
            }

            if (statusStr == criteria || prioStr == criteria) {
                std::println("- {} | {} | Status: {} | Priority: {}",
                    t->getId(), t->getTitle(), statusStr, prioStr);
                foundAny = true;
            }
        }
    }

    if (!foundAny) {
        std::println("No tasks found matching criteria: {}", criteria);
    }
}

void System::handleRemoveUser(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: remove-user <username>");

    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->getUsername() == args[0]) {
            users.erase(it);
            std::println("User '{}' removed from system.", args[0]);
            return;
        }
    }
    throw std::invalid_argument("Error: User not found.");
}

void System::handleRegister(const std::vector<std::string>& args) {
    if (args.size() != 3) throw std::invalid_argument("Usage: register <username> <password> <role>");

    for (const auto& u : users) {
        if (u->getUsername() == args[0]) throw std::invalid_argument("Error: Username already exists.");
    }

    std::string hashedPass = AuthenticationService::hashPassword(args[1]);
    std::string role = args[2];

    if (role == "Student") {
        users.push_back(std::make_shared<Student>(args[0], hashedPass));
    }
    else if (role == "TeachingAssistant") {
        users.push_back(std::make_shared<TeachingAssistant>(args[0], hashedPass));
    }
    else if (role == "Lecturer") {
        users.push_back(std::make_shared<Lecturer>(args[0], hashedPass));
    }
    else {
        throw std::invalid_argument("Error: Invalid role. Use Student, TeachingAssistant, or Lecturer.");
    }

    std::println("[System] User '{}' registered.", args[0]);
}

void System::handleStartStage(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: start-stage <stage_name>");

    auto currentUser = authService.getCurrentUser()->getUsername();

    for (const auto& p : projects) {
        if (p->hasMember(currentUser)) {
            auto stage = p->getStageByName(args[0]);
            if (!stage) {
                stage = std::make_shared<Stage>(args[0]);
                p->addStage(stage);
            }
            stage->startStage();
            std::println("[System] Stage '{}' started.", args[0]);
            return;
        }
    }
    throw std::invalid_argument("Error: You are not a member of any project to start a stage in.");
}

void System::handleFinishStage(const std::vector<std::string>& args) {
    if (args.size() != 1) throw std::invalid_argument("Usage: finish-stage <stage_name>");

    auto currentUser = authService.getCurrentUser()->getUsername();

    for (const auto& p : projects) {
        if (p->hasMember(currentUser)) {
            auto stage = p->getStageByName(args[0]);
            if (stage) {
                stage->finishStage();
                std::println("[System] Stage '{}' finished.", args[0]);
                return;
            }
        }
    }
    throw std::invalid_argument("Error: Stage not found in your projects.");
}

void System::handleMoveTaskToStage(const std::vector<std::string>& args) {
    if (args.size() != 2) throw std::invalid_argument("Usage: move-task-to-stage <task_id> <stage_name>");

    std::shared_ptr<Task> targetTask = nullptr;
    std::shared_ptr<Project> targetProject = nullptr;

    for (const auto& p : projects) {
        targetTask = p->getTaskById(args[0]);
        if (targetTask) {
            targetProject = p;
            break;
        }
    }

    if (!targetTask || !targetProject) throw std::invalid_argument("Error: Task not found.");

    auto stage = targetProject->getStageByName(args[1]);
    if (!stage) {
        stage = std::make_shared<Stage>(args[1]);
        targetProject->addStage(stage);
    }

    stage->addTask(targetTask);
    std::println("[System] Task {} moved to {}.", args[0], args[1]);
}

void System::handleStageReport() {
    auto currentUser = authService.getCurrentUser()->getUsername();
    bool foundAny = false;

    for (const auto& p : projects) {
        if (p->hasMember(currentUser)) {
            for (const auto& stage : p->getStages()) {
                if (stage->getStatus() == StageStatus::Active) {
                    foundAny = true;
                    int completed = 0;
                    for (const auto& t : stage->getTasks()) {
                        if (t->getStatus() == Status::Done) {
                            completed++;
                        }
                    }

                    std::println("Stage: {}", stage->getName());
                    std::println("Tasks: {}", stage->getTasks().size());
                    std::println("Completed: {}", completed);
                }
            }
        }
    }

    if (!foundAny) {
        std::println("No active stages found to report.");
    }
}

void System::handleSave() {

    std::ofstream userFile("users.txt");
    if (!userFile) {
        throw std::runtime_error("Error: Could not open users.txt for writing.");
    }

    for (const auto& u : users) {
        std::string roleStr;
        switch (u->getRole()) {
        case Role::Student: roleStr = "Student"; break;
        case Role::TeachingAssistant: roleStr = "TeachingAssistant"; break;
        case Role::Lecturer: roleStr = "Lecturer"; break;
        case Role::Administrator: roleStr = "Administrator"; break;
        default: roleStr = "Unknown";
        }

        userFile << u->getUsername() << "|"
            << u->getPasswordHash() << "|"
            << roleStr << "\n";
    }

    userFile.close();
    std::println("[System] User data saved successfully to users.txt.");

    std::ofstream projFile("projects.txt");
    if (!projFile) throw std::runtime_error("Error: Could not open projects.txt for writing.");

    for (const auto& p : projects) {
        projFile << p->getName() << "|"
            << p->getDescription() << "|"
            << p->getIsArchived() << "|"
            << p->getIsFinalized() << "|";

        bool first = true;
        for (const auto& m : p->getMembers()) {
            if (auto member = m.lock()) {
                if (!first) projFile << ",";
                projFile << member->getUsername();
                first = false;
            }
        }
        projFile << "\n";
    }
    projFile.close();
    std::println("[System] Project data saved successfully to projects.txt.");

    std::ofstream taskFile("tasks.txt");
    if (!taskFile) throw std::runtime_error("Error: Could not open tasks.txt for writing.");

    for (const auto& p : projects) {
        for (const auto& t : p->getTasks()) {
            std::string assigneeName = "Unassigned";
            if (auto a = t->getAssignee().lock()) {
                assigneeName = a->getUsername();
            }

            taskFile << t->getId() << "|"
                << p->getName() << "|" 
                << t->getTitle() << "|"
                << t->getDescription() << "|"
                << t->getDeadline() << "|"
                << t->getPoints() << "|"
                << static_cast<int>(t->getType()) << "|"
                << static_cast<int>(t->getPriority()) << "|"
                << static_cast<int>(t->getStatus()) << "|"
                << assigneeName << "|"
                << t->getGrade() << "\n";
        }
    }
    taskFile.close();
    std::println("[System] Task data saved successfully to tasks.txt.");
}

void System::handleLoad() {
    authService.logout();
    users.clear();
    projects.clear();

    std::ifstream userFile("users.txt");
    if (!userFile) {
        std::println("[System] No previous user data found. Starting fresh.");
        initializeAdmin();
        return;
    }

    std::string line;
    while (std::getline(userFile, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string username, hash, role;

        std::getline(ss, username, '|');
        std::getline(ss, hash, '|');
        std::getline(ss, role, '|');

        if (role == "Student") {
            users.push_back(std::make_shared<Student>(username, hash));
        }
        else if (role == "TeachingAssistant") {
            users.push_back(std::make_shared<TeachingAssistant>(username, hash));
        }
        else if (role == "Lecturer") {
            users.push_back(std::make_shared<Lecturer>(username, hash));
        }
        else if (role == "Administrator") {
            users.push_back(std::make_shared<Administrator>(username, hash));
        }
    }
    userFile.close();

    std::println("[System] User data loaded successfully from users.txt.");

    std::ifstream projFile("projects.txt");
    if (projFile) {
        std::string line;
        while (std::getline(projFile, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string name, desc, archStr, finStr, membersStr;

            std::getline(ss, name, '|');
            std::getline(ss, desc, '|');
            std::getline(ss, archStr, '|');
            std::getline(ss, finStr, '|');
            std::getline(ss, membersStr, '|');

            auto proj = std::make_shared<Project>(name, desc);
            proj->setArchived(archStr == "1");
            proj->setFinalized(finStr == "1");

            if (!membersStr.empty()) {
                std::stringstream memStream(membersStr);
                std::string memberName;
                while (std::getline(memStream, memberName, ',')) {
                    for (const auto& u : users) {
                        if (u->getUsername() == memberName) {
                            proj->addMember(u);
                            break;
                        }
                    }
                }
            }

            projects.push_back(proj);
        }
        projFile.close();
        std::println("[System] Project data loaded successfully from projects.txt.");

        std::ifstream taskFile("tasks.txt");
        if (taskFile) {
            std::string line;
            while (std::getline(taskFile, line)) {
                if (line.empty()) continue;

                std::stringstream ss(line);
                std::string id, projName, title, desc, deadline, pointsStr, typeStr, prioStr, statusStr, assigneeName, gradeStr;

                std::getline(ss, id, '|');
                std::getline(ss, projName, '|');
                std::getline(ss, title, '|');
                std::getline(ss, desc, '|');
                std::getline(ss, deadline, '|');
                std::getline(ss, pointsStr, '|');
                std::getline(ss, typeStr, '|');
                std::getline(ss, prioStr, '|');
                std::getline(ss, statusStr, '|');
                std::getline(ss, assigneeName, '|');
                std::getline(ss, gradeStr, '|');

                int points = 0, typeInt = 0, prioInt = 0, statusInt = 0, gradeInt = -1;

                std::stringstream(pointsStr) >> points;
                std::stringstream(typeStr) >> typeInt;
                std::stringstream(prioStr) >> prioInt;
                std::stringstream(statusStr) >> statusInt;
                std::stringstream(gradeStr) >> gradeInt;

                std::shared_ptr<Project> targetProj = nullptr;
                for (const auto& p : projects) {
                    if (p->getName() == projName) {
                        targetProj = p;
                        break;
                    }
                }

                if (targetProj) {
                    auto newTask = std::make_shared<Task>(title, desc, deadline, points,
                        static_cast<TaskType>(typeInt),
                        static_cast<Priority>(prioInt));

                    newTask->setId(id);

                    newTask->setStatus(static_cast<Status>(statusInt), "SystemLoad");
                    if (gradeInt != -1) {
                        newTask->setGrade(gradeInt, "SystemLoad");
                    }

                    if (assigneeName != "Unassigned") {
                        for (const auto& u : users) {
                            if (u->getUsername() == assigneeName) {
                                newTask->assignUser(u, "SystemLoad");
                                break;
                            }
                        }
                    }

                    targetProj->addTask(newTask);
                }
            }
            taskFile.close();
            std::println("[System] Task data loaded successfully from tasks.txt.");
        }
    }
}