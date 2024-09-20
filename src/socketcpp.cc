#include "socketcpp.h"

namespace ljh::socket {

// <sys/socket.h> <netinet/in.h>需要包含这两个头文件
Socket::Socket(int family, int type) {
    sockfd_ = ::socket(family, type, IPPROTO_TCP);
    if (sockfd_ < 0) {
        throw std::runtime_error(std::format("创建监听套接字失败, 错误码: {}, 错误信息: {}\n", errno, strerror(errno)));
    }
}

Socket::Socket(int connfd, std::string ip, unsigned short port) : sockfd_{connfd}, ip_addr_{ip}, port_{port} {}

bool Socket::operator==(const ljh::socket::Socket sock) {
    return sockfd_ == sock.sockfd_;
}

bool Socket::Bind(const std::string& host, unsigned short int port) {
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;

    // ip地址和端口要经过转换
    if (host.empty())
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        sockaddr.sin_addr.s_addr = inet_addr(host.c_str());  // 需要包含的头文件<arpa/inet.h>

    sockaddr.sin_port = htons(port);

    if (::bind(sockfd_, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        throw std::runtime_error(std::format("绑定失败, 错误码: {}, 错误信息: {}\n", errno, strerror(errno)));
    }

    ip_addr_ = host;
    port_ = port;

    return true;
}

bool Socket::Listen(int backlog) {
    if (::listen(sockfd_, backlog) < 0) {
        throw std::runtime_error(std::format("监听失败, 错误码: {}, 错误信息: {}\n", errno, strerror(errno)));
    }

    std::cout << std::format("Server is listening on {}:{}", ip_addr_, port_) << std::endl;
    return true;
}

Socket Socket::Accept(void) {
    struct sockaddr_in client_address;
    socklen_t client_addrlen = sizeof(client_address);

    int connfd = ::accept(sockfd_, (struct sockaddr*)&client_address, &client_addrlen);
    if (connfd < 0) {
        throw std::runtime_error(std::format("获取客户端连接失败, 错误码: {}, 错误信息: {}\n", errno, strerror(errno)));
    }

    char client_ip[INET_ADDRSTRLEN];  // IPv4地址长度
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_address.sin_port);
    std::cout << "Client connected - IP: " << client_ip << ", Port: " << client_port << std::endl;
    return Socket(connfd, client_ip, client_port);
}

bool Socket::Connect(const std::string& ip, unsigned short port) {
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);

    if (::connect(sockfd_, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        throw std::runtime_error(std::format("连接服务端失败, 错误码: {}, 错误信息: {}\n", errno, strerror(errno)));
        return false;
    }

    ip_addr_ = ip;
    port_ = port;
    return true;
}

std::string Socket::Recv(size_t n) const {
    char buffer[n]{};
    ::memset(buffer, '\0', n);
    int byte_num = recv(sockfd_, buffer, n, 0);
    if (byte_num < 0) {
        throw std::runtime_error(std::format("客户端连接异常, 错误码: {}, 错误信息: {}\n", errno, strerror(errno)));
    }

    return std::string{buffer, static_cast<size_t>(byte_num)};
}

bool Socket::Send(const std::string& message) {
    return ::send(sockfd_, message.c_str(), message.size(), 0);
}

// 需要包含<unistd.h>
void Socket::Close(void) {
    if (sockfd_ < 0) {
        throw std::runtime_error(std::format("关闭文件描述符失败, 错误码: {}, 错误信息: {}\n", errno, strerror(errno)));
    }
    if (::close(sockfd_) < 0) {
        throw std::runtime_error(std::format("关闭文件描述符失败, 错误码: {}, 错误信息: {}\n", errno, strerror(errno)));
    }
}

const std::string& Socket::GetIpAddr(void) {
    return ip_addr_;
}

}  // namespace ljh::socket
