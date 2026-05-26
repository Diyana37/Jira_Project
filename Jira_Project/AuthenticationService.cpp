#include "AuthenticationService.h"

std::string AuthenticationService::hashPassword(const std::string& password) {
    return "hash_" + password;
}

bool AuthenticationService::login(const std::string& username, const std::string& password, const std::vector<std::shared_ptr<User>>& allUsers) {
    std::string hashedPass = hashPassword(password);

    for (const auto& user : allUsers) {
        if (user->getUsername() == username && user->getPasswordHash() == hashedPass) {
            currentUser = user;
            return true;
        }
    }
    return false;
}

void AuthenticationService::logout() {
    currentUser = nullptr;
}

std::shared_ptr<User> AuthenticationService::getCurrentUser() const {
    return currentUser;
}

bool AuthenticationService::isLoggedIn() const {
    return currentUser != nullptr;
}
