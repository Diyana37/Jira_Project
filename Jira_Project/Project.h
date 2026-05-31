#pragma once
#include <string>
#include <vector>
#include <memory>
#include "User.h"

class Task;

class Project {
private:
    std::string name;
    std::string description;
    bool isArchived;
    bool isFinalized;

    std::vector<std::weak_ptr<User>> members;
    std::vector<std::shared_ptr<Task>> tasks;

public:
    Project(const std::string& projName, const std::string& projDesc = "");
    ~Project() = default;

    std::string getName() const;
    std::string getDescription() const;
    bool getIsArchived() const;
    bool getIsFinalized() const;

    void setArchived(bool archived);
    void setFinalized(bool finalized);

    void addMember(std::shared_ptr<User> user);
    bool hasMember(const std::string& username) const;
    const std::vector<std::weak_ptr<User>>& getMembers() const;

    void baddTask(std::shared_ptr<Task> task);
    std::shared_ptr<Task> getTaskById(const std::string& taskId) const;
    const std::vector<std::shared_ptr<Task>>& getTasks() const;
};