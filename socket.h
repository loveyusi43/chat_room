#ifndef __SOCKET_H
#define __SOCKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>


#include <format>
#include <stdexcept>
#include <iostream>

namespace ljh::socket {
class Socket {
public:
    Socket(int family, int type);

    Socket(int connfd, std::string ip, unsigned short port);

    bool Bind(const std::string& host, unsigned short int port);

    bool Listen(int backlog);

    Socket Accept(void);

private:
    int sockfd_;
    std::string ip_addr_;
    unsigned short int port_;
};
}  // namespace ljh::socket

#endif