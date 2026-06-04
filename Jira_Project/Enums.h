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
