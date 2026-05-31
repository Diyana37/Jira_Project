#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Enums.h"
#include "User.h"
#include "Comment.h"

class Task {
    static int nextId;
    std::string id;
    TaskType type;
    Priority priority;
    Status status;
    std::string title;
    std::string description;
    std::string deadline;
    int points;
    std::weak_ptr<User> assignee;
    int grade;
    std::vector<std::string> tags;
    std::vector<Comment> comments;
    std::vector<std::string> history;

    std::string getCurrentTimestamp() const;
    void logHistory(const std::string& action);

public:
    Task(const std::string& tTitle, const std::string& tDesc, 
        const std::string& tDeadline, int tPoints, TaskType tType, Priority tPriority);    
    ~Task() = default;

    std::string getId() const;
    TaskType getType() const;
    Priority getPriority() const;
    Status getStatus() const;
    std::weak_ptr<User> getAssignee() const;
    int getGrade() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getDeadline() const;
    int getPoints() const;

    void setStatus(Status newStatus, const std::string& modifierUsername);
    void assignUser(std::shared_ptr<User> user, const std::string& modifierUsername);
    void addComment(const std::string& author, const std::string& content);
    void addTag(const std::string& tag);
    void setGrade(int newGrade, const std::string& modifierUsername);
};