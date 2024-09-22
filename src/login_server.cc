#include <iostream>
#include <singleton.hpp>
#include <thread>

#include "socketcpp.h"
#include "user.h"

void HandleLogin(const ljh::socket::Socket&);

int main(int argc, char** argv) {
    ljh::socket::Socket login_socket{AF_INET, SOCK_STREAM};
    login_socket.Bind("127.0.0.1", 6666);
    login_socket.Listen(1024);
    login_socket.SetSockopt(SOL_SOCKET, SO_REUSEADDR, 1);
    login_socket.SetSockopt(SOL_SOCKET, SO_REUSEPORT, 1);

    while (true) {
        ljh::socket::Socket client = login_socket.Accept();
        std::thread(HandleLogin, std::ref(client)).detach();
    }

    return 0;
}

void HandleLogin(const ljh::socket::Socket& client) {
    for (int i = 0; i < 3; ++i) {
        std::string data = client.Recv(1024);
        std::size_t pos = data.find("-");
        std::string username = data.substr(0, pos);
        std::string password = data.substr(pos + 1);

        std::cout << std::format("username: {}, password: {}\n", username, password);

        bool state = Singleton<ljh::user::User>::Instence().Login(username, password);

        if (state) {
            client.Send("true");
            break;
        } else {
            client.Send("false");
        }
    }

    client.Close();
}