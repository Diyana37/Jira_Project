#include "Project.h"
#include "Task.h"
#include "Stage.h"
#include <algorithm>
#include <stdexcept>

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
    if (!user) {
        throw std::invalid_argument("You cannot add empty user!");
    }

    if (hasMember(user->getUsername())) {
        throw std::logic_error("This user is already a part of this project!");
    }

    members.push_back(user);
}

bool Project::hasMember(const std::string& username) const {
    if (username.empty()) {
        throw std::invalid_argument("Username cannot be empty!");
    }

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
    if (isArchived || isFinalized) {
        throw std::logic_error("You cannot add task to archived/finalized project!");
    }

    if (!task) {
        throw std::invalid_argument("The task cannot be empty!");
    }

    tasks.push_back(task);
}

std::shared_ptr<Task> Project::getTaskById(const std::string& taskId) const {
    if (taskId.empty()) {
        throw std::invalid_argument("Task ID cannot be empty!");
    }

    for (const auto& task : tasks) {
        if (task->getId() == taskId) {
            return task;
        }
    }

    throw std::out_of_range("Task with that ID was not found!");
}

const std::vector<std::shared_ptr<Task>>& Project::getTasks() const {
    return tasks;
}

void Project::addStage(std::shared_ptr<Stage> stage) {
    if (isArchived || isFinalized) {
        throw std::logic_error("You cannot add stage to archived/finalized project!");
    }

    if (!stage) {
        throw std::invalid_argument("The stage cannot be empty!");
    }

    stages.push_back(stage);
}

std::shared_ptr<Stage> Project::getStageByName(const std::string& stageName) const {
    if (stageName.empty()) {
        throw std::invalid_argument("Stage name cannot be empty!");
    }

    for (const auto& stage : stages) {
        if (stage->getName() == stageName) {
            return stage;
        }
    }

    throw std::out_of_range("Stage with that name was not found!");
}

const std::vector<std::shared_ptr<Stage>>& Project::getStages() const {
    return stages;
}