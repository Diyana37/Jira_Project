#pragma once
#include <string>

enum class Role {
    Student,
    TeachingAssistant,
    Lecturer,
    Administrator
};

enum class TaskType {
    Bug,
    Feature,
    Task,
    Improvement
};

enum class Priority {
    Low,
    Medium,
    High,
    Critical
};

enum class Status {
    ToDo,
    InProgress,
    InReview,
    Done
};

enum class StageStatus {
    Planned,
    NotStarted,
    Active,
    Finished
};

enum class CommandType {
    Login,
    Logout,
    ViewProfile,
    Help,
    Close,
    CreateProject,
    ArchiveProject,
    RemoveProject,
    FinalizeProject,
    AddUserToProject,
    JoinProject,
    ListProjects,
    ListAllProjects,
    CreateTask,
    AssignTask,
    ChangeStatus,
    AddComment,
    AddTag,
    ReviewTask,
    ApproveTask,
    GradeTask,
    ListTasks,
    ListAllTasks,
    MyTasks,
    UpcomingTasks,
    SearchTasks,
    FilterTasks,
    StageReport,
    StudentReport,
    StartStage,
    FinishStage,
    MoveTaskToStage,
    Save,
    Load,
    RemoveUser,
    Register,
    Unknown
};

enum class ProjectStatus {
    Active,
    Finished,
    InProgress,
    Unknown
};

TaskType parseTaskType(const std::string& str);
Priority parsePriority(const std::string& str);
Status parseStatus(const std::string& str);
Role parseRole(const std::string& str);
StageStatus parseStageStatus(const std::string& str);
ProjectStatus parseProjectStatus(const std::string& str);

std::string roleToString(Role r);
std::string statusToString(Status s);
std::string priorityToString(Priority p);
std::string stageStatusToString(StageStatus s);
std::string projectStatusToString(ProjectStatus p);
std::string taskTypeToString(TaskType t);

