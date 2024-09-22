#include <iostream>
#include <thread>

#include "socketcpp.h"

std::string Login(void);

void Chat(const std::string& username);

void ReceiveMessage(const ljh::socket::Socket& sock);
void SendMessage(const ljh::socket::Socket& sock);

int main(int argc, char** argv) {
    std::string username{Login()};
    std::cout << "登录成功" << std::endl;
    Chat(username);

    return 0;
}

std::string Login(void) {
    ljh::socket::Socket sock{AF_INET, SOCK_STREAM};
    sock.Connect("127.0.0.1", 6666);

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

    std::thread receive_thread{ReceiveMessage, std::ref(sock)};
    std::thread send_thread{SendMessage, std::ref(sock)};

    // while (true) {
    //     std::string message{};
    //     std::cout << ">> ";
    //     std::cin >> message;
    //     std::getline(std::cin, message);
    //     sock.Send(message);
    // }

    receive_thread.join();
    send_thread.join();
    sock.Close();
}

void ReceiveMessage(const ljh::socket::Socket& sock) {
    while (true) {
        std::string message{sock.Recv(1024)};
        std::cout << message << std::endl;
    }
}

void SendMessage(const ljh::socket::Socket& sock) {
    while (true) {
        std::string message{};
        std::cout << ">> ";
        std::cin >> message;
        if (!sock.Send(message)) {
            break;
        }
    }
}