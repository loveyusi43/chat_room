#include <singleton.hpp>
#include <iostream>

#include "socketcpp.h"
#include "user.h"

int main(int argc, char** argv) {
    ljh::socket::Socket login_socket{AF_INET, SOCK_STREAM};
    login_socket.Bind("127.0.0.1", 8080);
    login_socket.Listen(1024);

    while (true) {
        ljh::socket::Socket client = login_socket.Accept();

        std::string data = client.Recv(1024);

        std::string username = "ljh";
        std::string password = "654203";

        bool state = Singleton<ljh::user::User>::Instence().Login(username, password);

        if (state) {
            client.Send("登录成功");
        }else{
            client.Send("用户名或密码错误");
        }
    }

    return 0;
}