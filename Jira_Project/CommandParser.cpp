#include "CommandParser.h"

namespace {
    constexpr unsigned long hashString(const char* str) {
        unsigned long hash = 5381;
        while (*str) {
            hash = ((hash << 5) + hash) + *str;
            ++str;
        }
        return hash;
    }
}

CommandType CommandParser::stringToCommandType(const std::string& cmdStr) {
    switch (hashString(cmdStr.c_str())) {
    case hashString("login"): return CommandType::Login;
    case hashString("logout"): return CommandType::Logout;
    case hashString("view-profile"): return CommandType::ViewProfile;
    case hashString("help"): return CommandType::Help;
    case hashString("close"): return CommandType::Close;
    case hashString("create-project"): return CommandType::CreateProject;
    case hashString("archive-project"): return CommandType::ArchiveProject;
    case hashString("remove-project"): return CommandType::RemoveProject;
    case hashString("finalize-project"): return CommandType::FinalizeProject;
    case hashString("add-user-to-project"): return CommandType::AddUserToProject;
    case hashString("join-project"): return CommandType::JoinProject;
    case hashString("list-projects"): return CommandType::ListProjects;
    case hashString("list-all-projects"): return CommandType::ListAllProjects;
    case hashString("create-task"): return CommandType::CreateTask;
    case hashString("assign-task"): return CommandType::AssignTask;
    case hashString("change-status"): return CommandType::ChangeStatus;
    case hashString("add-comment"): return CommandType::AddComment;
    case hashString("add-tag"): return CommandType::AddTag;
    case hashString("review-task"): return CommandType::ReviewTask;
    case hashString("approve-task"): return CommandType::ApproveTask;
    case hashString("grade-task"): return CommandType::GradeTask;
    case hashString("list-tasks"): return CommandType::ListTasks;
    case hashString("list-all-tasks"): return CommandType::ListAllTasks;
    case hashString("my-tasks"): return CommandType::MyTasks;
    case hashString("upcoming-tasks"): return CommandType::UpcomingTasks;
    case hashString("search-tasks"): return CommandType::SearchTasks;
    case hashString("filter-tasks"): return CommandType::FilterTasks;
    case hashString("stage-report"): return CommandType::StageReport;
    case hashString("student-report"): return CommandType::StudentReport;
    case hashString("start-stage"): return CommandType::StartStage;
    case hashString("finish-stage"): return CommandType::FinishStage;
    case hashString("move-task-to-stage"): return CommandType::MoveTaskToStage;
    case hashString("save"): return CommandType::Save;
    case hashString("load"): return CommandType::Load;
    case hashString("remove-user"): return CommandType::RemoveUser;
    case hashString("register"): return CommandType::Register;
    default: return CommandType::Unknown;
    }
}

Command CommandParser::parse(const std::string& input) {
    Command cmd;
    cmd.type = CommandType::Unknown;

    if (input.empty()) return cmd;

    size_t start = input.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return cmd;

    size_t end = input.find_first_of(" \t\n\r", start);
    std::string token = input.substr(start, end - start);

    cmd.type = stringToCommandType(token);

    while (end != std::string::npos) {
        start = input.find_first_not_of(" \t\n\r", end);
        if (start == std::string::npos) break;
        end = input.find_first_of(" \t\n\r", start);
        cmd.args.push_back(input.substr(start, end - start));
    }

    return cmd;
}