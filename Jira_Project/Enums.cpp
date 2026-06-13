#include <iostream>
#include "Enums.h"

TaskType parseTaskType(const std::string& str) {
    if (str == "Feature") {
        return TaskType::Feature;
    }
    if (str == "Improvement") {
        return TaskType::Improvement;
    }
    if (str == "Bug") {
        return TaskType::Bug;
    }
    return TaskType::Task;
}

Priority parsePriority(const std::string& str) {
    if (str == "Low") {
        return Priority::Low;
    }
    if (str == "High") {
        return Priority::High;
    }
    if (str == "Critical") {
        return Priority::Critical;
    }
    return Priority::Medium;
}

Status parseStatus(const std::string& str) {
    if (str == "InProgress") {
        return Status::InProgress;
    }
    if (str == "InReview") {
        return Status::InReview;
    }
    if (str == "Done") {
        return Status::Done;
    }
    return Status::ToDo;
}

Role parseRole(const std::string& str) {
    if (str == "Student") {
        return Role::Student;
    }
    if (str == "TeachingAssistant") {
        return Role::TeachingAssistant;
    }
    if (str == "Lecturer") {
        return Role::Lecturer;
    }
    if (str == "Administrator") {
        return Role::Administrator;
    }
    throw std::invalid_argument("Invalid role.");
}

StageStatus parseStageStatus(const std::string& str) {
    if (str == "NotStarted") {
        return StageStatus::NotStarted;
    }
    if (str == "Active") {
        return StageStatus::Active;
    }
    if (str == "Finished") {
        return StageStatus::Finished;
    }
    return StageStatus::Planned;
}

ProjectStatus parseProjectStatus(const std::string& str) {
    if (str == "Finished") {
        return ProjectStatus::Finished;
    }
    if (str == "InProgress") {
        return ProjectStatus::InProgress;
    }
    if (str == "Unknown") {
        return ProjectStatus::Unknown;
    }
    return ProjectStatus::Active;
}

std::string roleToString(Role r) {
    if (r == Role::Student) {
        return "Student";
    }
    if (r == Role::TeachingAssistant) {
        return "TeachingAssistant";
    }
    if (r == Role::Lecturer) {
        return "Lecturer";
    }
    return "Administrator";
}

std::string statusToString(Status s) {
    if (s == Status::InProgress) {
        return "InProgress";
    }
    if (s == Status::InReview) {
        return "InReview";
    }
    if (s == Status::Done) {
        return "Done";
    }
    return "ToDo";
}

std::string priorityToString(Priority p) {
    if (p == Priority::Low) {
        return "Low";
    }
    if (p == Priority::High) {
        return "High";
    }
    if (p == Priority::Critical) {
        return "Critical";
    }
    return "Medium";
}

std::string stageStatusToString(StageStatus s) {
    if (s == StageStatus::NotStarted) {
        return "NotStarted";
    }
    if (s == StageStatus::Active) {
        return "Active";
    }
    if (s == StageStatus::Finished) {
        return "Finished";
    }
    return "Planned";
}

std::string projectStatusToString(ProjectStatus p) {
    if (p == ProjectStatus::Finished) {
        return "Finished";
    }
    if (p == ProjectStatus::InProgress) {
        return "InProgress";
    }
    if (p == ProjectStatus::Unknown) {
        return "Unknown";
    }
    return "Active";
}

std::string taskTypeToString(TaskType t) {
    if (t == TaskType::Feature) {
        return "Feature";
    }
    if (t == TaskType::Improvement) {
        return "Improvement";
    }
    if (t == TaskType::Bug) {
        return "Bug";
    }
    return "Task";
}