#ifndef __SOCKET_H
#define __SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#include <stdexcept>
#include <format>

namespace ljh::socket {
class Socket {
public:
    Socket(int family, int type);

    bool Bind(const std::string& host, int port);

private:
    int sockfd_;
};
}  // namespace ljh::socket

#endif