#define _CRT_SECURE_NO_WARNINGS
#include "Stage.h"
#include <ctime>
#include "Enums.h"

Stage::Stage(const std::string& stageName)
    : name(stageName), startDate("Not started"), endDate("Not finished"), status(StageStatus::Planned) {
}

std::string Stage::getCurrentTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::string timeStr = std::ctime(&now);
    if (!timeStr.empty() && timeStr.back() == '\n') {
        timeStr.pop_back();
    }
    return timeStr;
}

std::string Stage::getName() const { return name; }
std::string Stage::getStartDate() const { return startDate; }
std::string Stage::getEndDate() const { return endDate; }
StageStatus Stage::getStatus() const { return status; }
const std::vector<std::shared_ptr<Task>>& Stage::getTasks() const { return tasks; }

void Stage::startStage() {
    status = StageStatus::Active;
    startDate = getCurrentTimestamp();
}

void Stage::finishStage() {
    status = StageStatus::Finished;
    endDate = getCurrentTimestamp();
}

void Stage::addTask(std::shared_ptr<Task> task) {
    if (task) {
        tasks.push_back(task);
    }
}