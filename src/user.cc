#include "user.h"

ljh::user::User::User() {
    users_ = {
        {"ljh", "654203"},
        {"loveyusi", "3502"}};
}

bool ljh::user::User::Login(std::string username, std::string password) {
    if (users_.count(username) && (users_[username] == password)) {
        return true;
    }

    return false;
}
