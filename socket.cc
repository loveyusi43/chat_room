#include "socket.h"

namespace ljh::socket {

// <sys/socket.h> <netinet/in.h>需要包含这两个头文件
Socket::Socket(int family, int type) {
    sockfd_ = ::socket(family, type, IPPROTO_TCP);
    if (sockfd_ < 0) {
        throw std::runtime_error(std::format("创建监听套接字失败, 错误码: {}, 错误信息: {}\n", errno, strerror(errno)));
    }
}

bool Socket::Bind(const std::string& host, int port) {
    ;
}

}  // namespace ljh::socket
