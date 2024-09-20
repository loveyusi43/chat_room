#include <memory>
#include <thread>
#include <vector>

#include "socketcpp.h"

static std::unique_ptr<std::vector<ljh::socket::Socket>> clients_p{std::make_unique<std::vector<ljh::socket::Socket>>()};

void StartServer(void);

void Broadcast(std::string message, ljh::socket::Socket current_client);

void HandleClient(ljh::socket::Socket& client);

int main(int argc, char** argv) {
    StartServer();

    return 0;
}

void StartServer(void) {
    ljh::socket::Socket server{AF_INET6, SOCK_STREAM};

    server.Bind("127.0.0.1", 8080);

    server.Listen(1024);

    while (true) {
        ljh::socket::Socket client{server.Accept()};
        clients_p->emplace_back(client);

        std::thread{HandleClient, std::ref(client)}.detach();
    }

    server.Close();
}

void Broadcast(std::string message, ljh::socket::Socket current_client) {
    for (auto& client : *clients_p) {
        if (client == current_client) {
            continue;
        }
        client.Send(message);
    }
}

// client为何不能用const修饰未知
void HandleClient(ljh::socket::Socket& client) {
    while (true) {
        std::string data = client.Recv(1024);
        if (data.empty()) {
            std::cout << std::format("{}断开连接", client.GetIpAddr()) << std::endl;
            client.Close();
            break;
        }
    }
}