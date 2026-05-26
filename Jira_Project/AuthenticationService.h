#pragma once
#include <memory>
#include <string>
#include <vector>
#include "User.h"

class AuthenticationService {
private:
    std::shared_ptr<User> currentUser;

public:
    AuthenticationService() : currentUser(nullptr) {}

    static std::string hashPassword(const std::string& password);

    bool login(const std::string& username, const std::string& password, const std::vector<std::shared_ptr<User>>& allUsers);
    void logout();
    
    std::shared_ptr<User> getCurrentUser() const;
    bool isLoggedIn() const;
};
