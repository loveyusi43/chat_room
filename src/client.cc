#include <iostream>
#include <thread>

#include "socketcpp.h"

std::string Login(void);

void Chat(const std::string& username);

void Receive(const ljh::socket::Socket& sock);

int main(int argc, char** argv) {
    std::string username{Login()};

    Chat(username);

    return 0;
}

std::string Login(void) {
    ljh::socket::Socket sock{AF_INET, SOCK_STREAM};
    sock.Connect("127.0.0.1", 8080);

    std::string username{};
    std::string password{};

    std::cout << "username:";
    std::cin >> username;
    std::cout << "password:";
    std::cin >> password;

    std::string message = username + "-" + password;

    sock.Send(message);

    std::string data = sock.Recv(1024);

    std::cout << data;

    sock.Close();
    return username;
}

void Chat(const std::string& username) {
    ljh::socket::Socket sock{AF_INET, SOCK_STREAM};

    std::string ip_addr = "127.0.0.1";
    unsigned short port = 7777;
    if (!sock.Connect(ip_addr, port)) {
        return;
    }

    sock.Send(username);

    std::thread receive_t{Receive};

    while (true) {
        std::string message{};
        std::cout << ">> ";
        std::cin >> message;
        sock.Send(message);
    }

    sock.Close();
}

void Receive(const ljh::socket::Socket& sock) {
    while (true) {
        std::string message{sock.Recv(1024)};
        std::cout << message;
    }
}