#ifndef __USER_H
#define __USER_H

#include <string>
#include <unordered_map>

#include "singleton.hpp"

namespace ljh::user {
class User {
    friend class Singleton<User>;

public:
    bool Login(std::string username, std::string password);

private:
    User();
    User(const User&) = delete;
    User& operator=(const User&) = delete;

private:
    std::unordered_map<std::string, std::string> users_;
};
}  // namespace ljh::user

#endif  // __USER_H