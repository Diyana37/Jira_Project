#pragma once
#include <string>
#include <vector>
#include <memory>
#include "User.h"

class Project {
private:
    std::string name;
    std::string description;
    bool isArchived;
    bool isFinalized;

    std::vector<std::weak_ptr<User>> members;

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
};