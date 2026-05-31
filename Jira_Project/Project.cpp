#include "Project.h"
#include "Task.h"
#include <algorithm>

Project::Project(const std::string& projName, const std::string& projDesc)
    : name(projName), description(projDesc), isArchived(false), isFinalized(false) {
}

std::string Project::getName() const { 
    return name; 
}
std::string Project::getDescription() const { 
    return description; 
}
bool Project::getIsArchived() const {
    return isArchived; 
}
bool Project::getIsFinalized() const { 
    return isFinalized; 
}

void Project::setArchived(bool archived) { 
    isArchived = archived; 
}
void Project::setFinalized(bool finalized) { 
    isFinalized = finalized; 
}

void Project::addMember(std::shared_ptr<User> user) {
    if (!user) return;

    if (!hasMember(user->getUsername())) {
        members.push_back(user);
    }
}

bool Project::hasMember(const std::string& username) const {
    for (const auto& weakUser : members) {
        if (auto sharedUser = weakUser.lock()) {
            if (sharedUser->getUsername() == username) {
                return true;
            }
        }
    }
    return false;
}

const std::vector<std::weak_ptr<User>>& Project::getMembers() const {
    return members;
}

void Project::addTask(std::shared_ptr<Task> task) {
    if (task) {
        tasks.push_back(task);
    }
}

std::shared_ptr<Task> Project::getTaskById(const std::string& taskId) const {
    for (const auto& task : tasks) {
        if (task->getId() == taskId) {
            return task;
        }
    }
    return nullptr; 
}

const std::vector<std::shared_ptr<Task>>& Project::getTasks() const {
    return tasks;
}