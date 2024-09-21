#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

#include "socketcpp.h"

std::unique_ptr<std::unordered_map<std::string, ljh::socket::Socket>> clients_p{
    std::make_unique<std::unordered_map<std::string, ljh::socket::Socket>>()};

void StartServer(void);

void Broadcast(std::string message, const std::string& username);

void HandleClient(std::string username);

int main(int argc, char** argv) {
    StartServer();
    return 0;
}

void StartServer(void) {
    ljh::socket::Socket server{AF_INET, SOCK_STREAM};
    server.Bind("127.0.0.1", 7777);
    server.Listen(1024);

    while (true) {
        ljh::socket::Socket client{server.Accept()};
        std::string username = client.Recv(1024);
        (*clients_p)[username] = client;

        std::thread{HandleClient, username}.detach();
    }

    server.Close();
}

void HandleClient(std::string username) {
    while (true) {
        std::string data = (*clients_p)[username].Recv(1024);
        if (data.empty()) {
            std::cout << std::format("[{}@{}] 断开连接", username, (*clients_p)[username].GetIpAddr()) << std::endl;
            (*clients_p)[username].Close();
            clients_p->erase(username);
            break;
        }
        Broadcast(data, username);
    }
}

void Broadcast(std::string message, const std::string& username) {
    for (const auto& [name, client] : (*clients_p)) {
        if (name == username) {
            continue;
        }
        client.Send(message);
    }
}