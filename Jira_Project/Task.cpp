#define _CRT_SECURE_NO_WARNINGS
#include "Task.h"
#include <ctime>
#include <stdexcept>

int Task::nextId = 100;

Task::Task(const std::string& tTitle, const std::string& tDesc,
    const std::string& tDeadline, int tPoints, TaskType tType, Priority tPriority)
    : title(tTitle), description(tDesc), deadline(tDeadline), 
    points(tPoints), type(tType), priority(tPriority), status(Status::ToDo), grade(-1) {

    id = "FMI-" + std::to_string(nextId++);
    logHistory("Task created.");
}

std::string Task::getCurrentTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::string timeStr = std::ctime(&now);

    if (!timeStr.empty() && timeStr.back() == '\n') {
        timeStr.pop_back();
    }

    return timeStr;
}

void Task::logHistory(const std::string& action) {
    history.push_back("[" + getCurrentTimestamp() + "] " + action);
}

std::string Task::getId() const { 
    return id; 
}

void Task::setId(const std::string& newId) {
    if (newId.empty()) {
        throw std::invalid_argument("New тask ID cannot be empty.");
    }

    id = newId;
}

TaskType Task::getType() const { 
    return type; 
}

Priority Task::getPriority() const {
    return priority; 
}

Status Task::getStatus() const { 
    return status; 
}
std::weak_ptr<User> Task::getAssignee() const { 
    return assignee;
}

int Task::getGrade() const {
    return grade; 
}

std::string Task::getTitle() const { 
    return title;
}

std::string Task::getDescription() const { 
    return description;
}

std::string Task::getDeadline() const {
    return deadline;
}

int Task::getPoints() const { 
    return points; 
}

void Task::setStatus(Status newStatus, const std::string& modifierUsername) {
    if (modifierUsername.empty()) {
        throw std::invalid_argument("Modifier username cannot be empty when changing status!");
    }

    if (status != newStatus) {
        status = newStatus;
        logHistory("Status updated to " + std::to_string(static_cast<int>(newStatus)) + " by " + modifierUsername);
    }
}

void Task::assignUser(std::shared_ptr<User> user, const std::string& modifierUsername) {
    if (!user) {
        throw std::invalid_argument("Cannot assign an empty user to the task!");
    }
    if (modifierUsername.empty()) {
        throw std::invalid_argument("Modifier username cannot be empty when assigning a user!");
    }

    assignee = user;
    logHistory("Assigned to " + user->getUsername() + " by " + modifierUsername);
}

void Task::addComment(const std::string& author, const std::string& content) {
    if (author.empty()) {
        throw std::invalid_argument("Comment author cannot be empty.");
    }
    if (content.empty()) {
        throw std::invalid_argument("Comment content cannot be empty.");
    }

    comments.push_back({ author, content, getCurrentTimestamp() });
    logHistory("Comment added by " + author);
}

void Task::addTag(const std::string& tag) {
    if (tag.empty()) {
        throw std::invalid_argument("Tag cannot be empty.");
    }

    tags.push_back(tag);
    logHistory("Tag '" + tag + "' added.");
}

void Task::setGrade(int newGrade, const std::string& modifierUsername) {
    if (modifierUsername.empty()) {
        throw std::invalid_argument("Modifier username cannot be empty when grading.");
    }

    if (newGrade < 0) {
        throw std::invalid_argument("Grade cannot be negative.");
    }

    grade = newGrade;
    logHistory("Graded with " + std::to_string(grade) + " by " + modifierUsername);
}