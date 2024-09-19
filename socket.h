#ifndef __SOCKET_H
#define __SOCKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <format>
#include <iostream>
#include <stdexcept>

namespace ljh::socket {
class Socket {
public:
    Socket(int family, int type);

    Socket(int connfd, std::string ip, unsigned short port);

    bool operator==(const ljh::socket::Socket sock);

    bool Bind(const std::string& host, unsigned short int port);

    bool Listen(int backlog);

    Socket Accept(void);

    std::string Recv(size_t);

    bool Send(const std::string& message);

    void Close(void);

    const std::string& GetIpAddr(void);

private:
    int sockfd_;
    std::string ip_addr_;
    unsigned short int port_;
};
}  // namespace ljh::socket

#endif