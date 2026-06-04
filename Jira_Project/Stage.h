#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Task.h"

class Stage {
private:
    std::string name;
    std::string startDate;
    std::string endDate;
    StageStatus status;
    std::vector<std::shared_ptr<Task>> tasks;

    std::string getCurrentTimestamp() const;

public:
    Stage(const std::string& stageName);
    ~Stage() = default;

    std::string getName() const;
    std::string getStartDate() const;
    std::string getEndDate() const;
    StageStatus getStatus() const;
    const std::vector<std::shared_ptr<Task>>& getTasks() const;

    void startStage();
    void finishStage();
    void addTask(std::shared_ptr<Task> task);
};