# chat_room

# 版本规划

## VERSION 1.0

一个有趣且简单的网络编程案例是创建一个简单的聊天室应用，用户可以在同一局域网中互相发送消息。我们可以使用 Python 的 `socket` 和 `threading` 模块来实现服务器和客户端。以下是这个案例的基本流程：

- **服务器端：** 监听来自多个客户端的连接，接收消息，并将消息广播给其他所有客户端。
- **客户端：** 连接到服务器，发送消息，并接收来自其他客户端的消息。

### 实现步骤：

1. 创建一个服务器脚本，能够处理多个客户端连接。
2. 创建一个客户端脚本，可以连接到服务器并与其他客户端通信。
3. 通过多线程来处理多个客户端同时发送和接收消息。

### 伪代码：
- 服务器：
  - 启动服务器，绑定 IP 和端口。
  - 监听多个客户端的连接。
  - 为每个客户端启动一个线程，处理它的消息收发。
  - 将消息广播给其他客户端。

- 客户端：
  - 连接到服务器。
  - 启动两个线程：一个发送消息，另一个接收来自服务器的消息。

### 服务器端代码

```python
import socket
import threading

# 服务器地址和端口
HOST = '127.0.0.1'  
PORT = 12345         

# 处理客户端的连接和消息
def handle_client(client_socket, client_address):
    print(f"[NEW CONNECTION] {client_address} connected.")
    while True:
        try:
            # 接收消息
            message = client_socket.recv(1024).decode('utf-8')
            if not message:
                break
            print(f"[{client_address}] {message}")
            # 广播给其他客户端
            broadcast(message, client_socket)
        except:
            break

    client_socket.close()

# 广播消息给所有连接的客户端
def broadcast(message, current_client_socket):
    for client in clients:
        if client != current_client_socket:
            try:
                client.send(message.encode('utf-8'))
            except:
                clients.remove(client)

# 启动服务器
def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen()
    print(f"[LISTENING] Server is listening on {HOST}:{PORT}")
    
    while True:
        client_socket, client_address = server.accept()
        clients.append(client_socket)
        thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.active_count() - 1}")

# 客户端列表
clients = []

if __name__ == "__main__":
    start_server()
```

### 客户端代码

```python
import socket
import threading

# 服务器地址和端口
HOST = '127.0.0.1'  
PORT = 12345         

# 从服务器接收消息
def receive_messages(client_socket):
    while True:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if message:
                print(message)
        except:
            print("An error occurred.")
            client_socket.close()
            break

# 发送消息到服务器
def send_messages(client_socket):
    while True:
        message = input("")
        client_socket.send(message.encode('utf-8'))

# 连接服务器
def start_client():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((HOST, PORT))
    
    # 启动两个线程，一个用于接收消息，一个用于发送消息
    receive_thread = threading.Thread(target=receive_messages, args=(client,))
    send_thread = threading.Thread(target=send_messages, args=(client,))
    
    receive_thread.start()
    send_thread.start()

if __name__ == "__main__":
    start_client()
```

### 运行说明：
1. 先运行服务器端脚本，服务器会监听新的客户端连接。
2. 然后在不同的终端运行多个客户端脚本，客户端可以发送消息并接收其他客户端的消息。

### 下一步建议：
**a.** 添加更复杂的功能，例如用户名、私聊功能。  
**b.** 使用 `asyncio` 替代 `threading` 来提高并发处理效率。

## VERSION 2.0

为了增强这个聊天室应用，可以添加**用户名**和**私聊功能**。下面我们将对服务器端和客户端进行一些改进，使每个客户端可以设置用户名并允许用户私聊某个特定的用户。

### 改进的功能：
1. **用户名功能：** 每个客户端连接时先输入用户名，之后的消息会显示用户名。
2. **私聊功能：** 用户可以通过输入 `/w 用户名 消息` 来发送私信，只有该特定用户会收到消息。

### 改进后的实现步骤：
- **服务器端：** 
  - 为每个客户端分配一个用户名。
  - 处理私聊消息：解析消息中是否包含私聊指令 (`/w`)。
  - 广播消息时，检查是否是私聊消息，如果是，则只发送给指定用户。

- **客户端：** 
  - 用户连接时输入用户名。
  - 用户可以输入私聊指令 `/w 用户名 消息` 来发送私信。

### 改进后的服务器端代码：

```python
import socket
import threading

HOST = '127.0.0.1'
PORT = 12345

# 客户端列表，存储用户名和套接字
clients = {}

# 广播消息给所有客户端，或特定客户端（用于私聊）
def broadcast(message, current_client_socket=None, recipient=None):
    if recipient:  # 私聊，发送给指定用户
        for client_socket, username in clients.items():
            if username == recipient:
                try:
                    client_socket.send(message.encode('utf-8'))
                except:
                    client_socket.close()
                    remove_client(client_socket)
        return

    # 广播给所有客户端
    for client_socket in clients.keys():
        if client_socket != current_client_socket:
            try:
                client_socket.send(message.encode('utf-8'))
            except:
                client_socket.close()
                remove_client(client_socket)

# 移除断开连接的客户端
def remove_client(client_socket):
    if client_socket in clients:
        del clients[client_socket]

# 处理客户端的消息收发
def handle_client(client_socket, client_address):
    print(f"[NEW CONNECTION] {client_address} connected.")
    
    # 要求客户端输入用户名
    client_socket.send("Please enter your username:".encode('utf-8'))
    username = client_socket.recv(1024).decode('utf-8')
    clients[client_socket] = username
    broadcast(f"{username} has joined the chat!")
    
    while True:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if not message:
                break

            # 检查是否是私聊消息
            if message.startswith("/w"):
                parts = message.split(" ", 2)
                if len(parts) < 3:
                    client_socket.send("Invalid private message format. Use: /w <username> <message>".encode('utf-8'))
                else:
                    recipient_username = parts[1]
                    private_message = f"[Private from {username}] {parts[2]}"
                    broadcast(private_message, current_client_socket=client_socket, recipient=recipient_username)
            else:
                # 广播消息
                broadcast(f"[{username}] {message}", current_client_socket=client_socket)
        except:
            break

    client_socket.close()
    remove_client(client_socket)
    broadcast(f"{username} has left the chat.")
    print(f"[DISCONNECTED] {client_address} disconnected.")

# 启动服务器
def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen()
    print(f"[LISTENING] Server is listening on {HOST}:{PORT}")
    
    while True:
        client_socket, client_address = server.accept()
        thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.active_count() - 1}")

if __name__ == "__main__":
    start_server()
```

### 改进后的客户端代码：

```python
import socket
import threading

HOST = '127.0.0.1'
PORT = 12345

# 接收服务器的消息
def receive_messages(client_socket):
    while True:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if message:
                print(message)
        except:
            print("An error occurred.")
            client_socket.close()
            break

# 发送消息到服务器
def send_messages(client_socket):
    while True:
        message = input("")
        client_socket.send(message.encode('utf-8'))

# 连接服务器
def start_client():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((HOST, PORT))
    
    # 接收并显示服务器的消息
    receive_thread = threading.Thread(target=receive_messages, args=(client,))
    receive_thread.start()
    
    # 发送用户名到服务器
    username = input("Enter your username: ")
    client.send(username.encode('utf-8'))

    # 启动发送消息线程
    send_thread = threading.Thread(target=send_messages, args=(client,))
    send_thread.start()

if __name__ == "__main__":
    start_client()
```

### 新功能说明：
1. 用户首次连接时会被要求输入用户名，之后每条消息都会附带用户名。
2. 通过输入 `/w <用户名> <消息>`，用户可以向指定用户名的用户发送私聊消息。

### 下一步建议：
**a.** 增加服务器端的日志记录功能，用于记录聊天历史。  
**b.** 使用 JSON 格式进行消息的序列化和反序列化，以便扩展更多功能。

## VERSION 3.0

为了进一步改进聊天室应用，我们可以使用 **JSON** 格式来序列化和反序列化消息，从而更灵活地扩展功能。通过 JSON，消息可以包含不同的类型（如普通消息、私聊消息、系统通知等），并且容易解析和处理。

### 改进内容：
1. **消息格式标准化**：将所有的消息（如普通消息、私聊消息、系统通知）包装成 JSON 格式传输，包含类型和内容。
2. **可扩展性**：未来可以很容易地扩展更多功能，比如加入文件传输、表情符号等。

### 消息的 JSON 格式：
每条消息可以包含以下信息：
- `type`：消息类型（如 `message`、`private_message`、`notification`）。
- `username`：发送消息的用户名。
- `content`：消息的具体内容（文本、图片、文件等）。
- `recipient`（可选）：用于私聊的目标用户名。

示例 JSON 格式：
```json
{
    "type": "message",
    "username": "Alice",
    "content": "Hello, everyone!"
}
```

对于私聊：
```json
{
    "type": "private_message",
    "username": "Alice",
    "content": "Hi Bob, this is a private message.",
    "recipient": "Bob"
}
```

### 改进后的服务器端代码：

```python
import socket
import threading
import json

HOST = '127.0.0.1'
PORT = 12345

clients = {}

# 广播消息给所有客户端，或发送给特定客户端（用于私聊）
def broadcast(message_data, current_client_socket=None, recipient=None):
    message_json = json.dumps(message_data)
    if recipient:
        for client_socket, username in clients.items():
            if username == recipient:
                try:
                    client_socket.send(message_json.encode('utf-8'))
                except:
                    client_socket.close()
                    remove_client(client_socket)
        return

    for client_socket in clients.keys():
        if client_socket != current_client_socket:
            try:
                client_socket.send(message_json.encode('utf-8'))
            except:
                client_socket.close()
                remove_client(client_socket)

# 移除客户端
def remove_client(client_socket):
    if client_socket in clients:
        del clients[client_socket]

# 处理客户端的连接和消息
def handle_client(client_socket, client_address):
    print(f"[NEW CONNECTION] {client_address} connected.")
    
    # 请求用户名
    client_socket.send(json.dumps({"type": "system", "content": "Please enter your username:"}).encode('utf-8'))
    username = client_socket.recv(1024).decode('utf-8')
    username = json.loads(username)["content"]
    clients[client_socket] = username
    broadcast({"type": "notification", "content": f"{username} has joined the chat!"})
    
    while True:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if not message:
                break

            message_data = json.loads(message)
            if message_data["type"] == "private_message":
                recipient_username = message_data["recipient"]
                broadcast(message_data, current_client_socket=client_socket, recipient=recipient_username)
            else:
                broadcast(message_data, current_client_socket=client_socket)

        except:
            break

    client_socket.close()
    remove_client(client_socket)
    broadcast({"type": "notification", "content": f"{username} has left the chat."})
    print(f"[DISCONNECTED] {client_address} disconnected.")

# 启动服务器
def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen()
    print(f"[LISTENING] Server is listening on {HOST}:{PORT}")
    
    while True:
        client_socket, client_address = server.accept()
        thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.active_count() - 1}")

if __name__ == "__main__":
    start_server()
```

### 改进后的客户端代码：

```python
import socket
import threading
import json

HOST = '127.0.0.1'
PORT = 12345

# 接收消息
def receive_messages(client_socket):
    while True:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if message:
                message_data = json.loads(message)
                if message_data["type"] == "notification":
                    print(f"[SYSTEM] {message_data['content']}")
                elif message_data["type"] == "private_message":
                    print(f"[Private from {message_data['username']}] {message_data['content']}")
                else:
                    print(f"[{message_data['username']}] {message_data['content']}")
        except:
            print("An error occurred.")
            client_socket.close()
            break

# 发送消息
def send_messages(client_socket, username):
    while True:
        message = input("")
        if message.startswith("/w"):  # 私聊
            parts = message.split(" ", 2)
            if len(parts) < 3:
                print("Invalid private message format. Use: /w <username> <message>")
            else:
                recipient_username = parts[1]
                content = parts[2]
                message_data = {
                    "type": "private_message",
                    "username": username,
                    "content": content,
                    "recipient": recipient_username
                }
                client_socket.send(json.dumps(message_data).encode('utf-8'))
        else:
            message_data = {
                "type": "message",
                "username": username,
                "content": message
            }
            client_socket.send(json.dumps(message_data).encode('utf-8'))

# 连接服务器
def start_client():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((HOST, PORT))
    
    # 启动接收消息线程
    receive_thread = threading.Thread(target=receive_messages, args=(client,))
    receive_thread.start()
    
    # 输入用户名并发送给服务器
    username = input("Enter your username: ")
    client.send(json.dumps({"type": "username", "content": username}).encode('utf-8'))

    # 启动发送消息线程
    send_thread = threading.Thread(target=send_messages, args=(client, username))
    send_thread.start()

if __name__ == "__main__":
    start_client()
```

### 新功能说明：
1. **JSON 格式传输**：所有消息都以 JSON 格式传输，确保消息类型和内容清晰明了，便于扩展更多功能。
2. **消息类型**：支持不同类型的消息，如普通消息、私聊消息、系统通知等。
3. **私聊功能**：输入 `/w <用户名> <消息>` 发送私聊消息。

### 下一步建议：

**a.** 为聊天室应用添加**消息历史保存功能**，使用文件或数据库存储所有聊天记录。  
**b.** 增加**用户状态管理**，显示在线用户列表以及用户的在线/离线状态。

## VERSION 4.0

为了增加聊天室的**用户状态管理**功能，我们可以改进应用，使其支持显示当前**在线用户列表**以及用户的**在线/离线状态**。这可以通过在服务器端维护一个在线用户的列表，并通过广播用户的连接状态变化来实现。

### 实现思路：
1. **用户状态管理**：服务器维护一个在线用户列表。每次有用户连接或断开连接时，更新列表并通知所有在线用户。
2. **在线用户列表**：客户端可以请求查看当前在线的所有用户。
3. **用户状态通知**：当有用户上线或离线时，服务器向所有用户发送系统通知，显示该用户的状态变化。

### 改进后的实现步骤：
- **服务器端**： 
  - 维护在线用户列表，向客户端广播用户上线、下线通知。
  - 支持客户端请求在线用户列表的功能。
  
- **客户端**： 
  - 可以通过输入特定指令（如 `/users`）来获取当前在线的用户列表。
  - 每当有用户上线或下线时，收到相应的系统通知。

### 改进后的服务器端代码：

```python
import socket
import threading
import json

HOST = '127.0.0.1'
PORT = 12345

clients = {}  # 存储每个连接的套接字和用户名

# 广播消息给所有客户端，或发送给特定客户端
def broadcast(message_data, current_client_socket=None, recipient=None):
    message_json = json.dumps(message_data)
    if recipient:
        for client_socket, username in clients.items():
            if username == recipient:
                try:
                    client_socket.send(message_json.encode('utf-8'))
                except:
                    client_socket.close()
                    remove_client(client_socket)
        return

    for client_socket in clients.keys():
        if client_socket != current_client_socket:
            try:
                client_socket.send(message_json.encode('utf-8'))
            except:
                client_socket.close()
                remove_client(client_socket)

# 发送当前在线用户列表
def send_online_users(client_socket):
    users = list(clients.values())
    online_users_message = {
        "type": "online_users",
        "content": users
    }
    client_socket.send(json.dumps(online_users_message).encode('utf-8'))

# 移除客户端
def remove_client(client_socket):
    if client_socket in clients:
        del clients[client_socket]

# 处理客户端的消息
def handle_client(client_socket, client_address):
    print(f"[NEW CONNECTION] {client_address} connected.")
    
    # 请求用户名
    client_socket.send(json.dumps({"type": "system", "content": "Please enter your username:"}).encode('utf-8'))
    username = client_socket.recv(1024).decode('utf-8')
    username = json.loads(username)["content"]
    clients[client_socket] = username
    
    # 广播用户上线
    broadcast({"type": "notification", "content": f"{username} has joined the chat!"})
    print(f"[ACTIVE USERS] {list(clients.values())}")
    
    # 主循环，处理消息
    while True:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if not message:
                break

            message_data = json.loads(message)
            
            # 如果用户请求查看在线用户
            if message_data["type"] == "get_users":
                send_online_users(client_socket)
            elif message_data["type"] == "private_message":
                recipient_username = message_data["recipient"]
                broadcast(message_data, current_client_socket=client_socket, recipient=recipient_username)
            else:
                broadcast(message_data, current_client_socket=client_socket)
        except:
            break

    # 用户离线
    client_socket.close()
    remove_client(client_socket)
    broadcast({"type": "notification", "content": f"{username} has left the chat."})
    print(f"[DISCONNECTED] {client_address} disconnected. [ACTIVE USERS] {list(clients.values())}")

# 启动服务器
def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen()
    print(f"[LISTENING] Server is listening on {HOST}:{PORT}")
    
    while True:
        client_socket, client_address = server.accept()
        thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.active_count() - 1}")

if __name__ == "__main__":
    start_server()
```

### 改进后的客户端代码：

```python
import socket
import threading
import json

HOST = '127.0.0.1'
PORT = 12345

# 接收消息
def receive_messages(client_socket):
    while True:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if message:
                message_data = json.loads(message)
                if message_data["type"] == "notification":
                    print(f"[SYSTEM] {message_data['content']}")
                elif message_data["type"] == "private_message":
                    print(f"[Private from {message_data['username']}] {message_data['content']}")
                elif message_data["type"] == "online_users":
                    print(f"[ONLINE USERS] {', '.join(message_data['content'])}")
                else:
                    print(f"[{message_data['username']}] {message_data['content']}")
        except:
            print("An error occurred.")
            client_socket.close()
            break

# 发送消息
def send_messages(client_socket, username):
    while True:
        message = input("")
        if message == "/users":  # 请求查看在线用户
            message_data = {
                "type": "get_users"
            }
            client_socket.send(json.dumps(message_data).encode('utf-8'))
        elif message.startswith("/w"):  # 私聊
            parts = message.split(" ", 2)
            if len(parts) < 3:
                print("Invalid private message format. Use: /w <username> <message>")
            else:
                recipient_username = parts[1]
                content = parts[2]
                message_data = {
                    "type": "private_message",
                    "username": username,
                    "content": content,
                    "recipient": recipient_username
                }
                client_socket.send(json.dumps(message_data).encode('utf-8'))
        else:
            message_data = {
                "type": "message",
                "username": username,
                "content": message
            }
            client_socket.send(json.dumps(message_data).encode('utf-8'))

# 连接服务器
def start_client():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((HOST, PORT))
    
    # 启动接收消息线程
    receive_thread = threading.Thread(target=receive_messages, args=(client,))
    receive_thread.start()
    
    # 输入用户名并发送给服务器
    username = input("Enter your username: ")
    client.send(json.dumps({"type": "username", "content": username}).encode('utf-8'))

    # 启动发送消息线程
    send_thread = threading.Thread(target=send_messages, args=(client, username))
    send_thread.start()

if __name__ == "__main__":
    start_client()
```

### 新功能说明：
1. **在线用户列表**：客户端可以通过输入 `/users` 来获取当前在线的用户列表。
2. **用户上线和下线通知**：每当有用户加入或离开聊天室时，所有在线用户会收到系统通知。
3. **扩展性**：通过 JSON 传输不同类型的消息，使得功能扩展更为方便。

### 下一步建议：
**a.** 添加**消息历史记录功能**，将聊天记录保存在服务器文件或数据库中，以便用户可以回顾以前的对话。  
**b.** 为应用添加**用户认证功能**，允许用户通过用户名和密码登录。

## VERSION 5.0

为了为聊天室应用添加**消息历史记录功能**，我们可以将聊天记录保存到服务器端的一个文件或数据库中，以便用户可以回顾以前的对话。我们将采用以下步骤：

### 实现思路：
1. **消息存储**：每当服务器接收到消息时，除了转发给其他用户外，也将消息存储到一个文本文件中。
2. **消息加载**：当新用户连接时，服务器可以从文件中读取先前的消息记录，并发送给该用户。
3. **消息格式**：每条消息都会以特定格式保存，如包含时间戳、用户名、消息内容。

### 改进后的功能：
- **消息保存**：服务器保存所有的聊天记录到文件。
- **消息加载**：当用户连接时，发送之前的消息历史给该用户。
- **时间戳**：为每条消息添加时间戳，便于查看消息时间。

### 改进后的服务器端代码：

```python
import socket
import threading
import json
import os
from datetime import datetime

HOST = '127.0.0.1'
PORT = 12345
LOG_FILE = 'chat_history.txt'  # 消息历史记录文件

clients = {}  # 存储每个连接的套接字和用户名

# 广播消息给所有客户端，或发送给特定客户端
def broadcast(message_data, current_client_socket=None, recipient=None):
    message_json = json.dumps(message_data)
    if recipient:
        for client_socket, username in clients.items():
            if username == recipient:
                try:
                    client_socket.send(message_json.encode('utf-8'))
                except:
                    client_socket.close()
                    remove_client(client_socket)
        return

    for client_socket in clients.keys():
        if client_socket != current_client_socket:
            try:
                client_socket.send(message_json.encode('utf-8'))
            except:
                client_socket.close()
                remove_client(client_socket)

# 保存消息到日志文件
def save_message(message_data):
    with open(LOG_FILE, 'a') as f:
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        log_message = f"[{timestamp}] {message_data['username']}: {message_data['content']}\n"
        f.write(log_message)

# 发送消息历史给新连接的用户
def send_message_history(client_socket):
    if os.path.exists(LOG_FILE):
        with open(LOG_FILE, 'r') as f:
            history = f.read()
            client_socket.send(json.dumps({"type": "history", "content": history}).encode('utf-8'))

# 移除客户端
def remove_client(client_socket):
    if client_socket in clients:
        del clients[client_socket]

# 处理客户端的消息
def handle_client(client_socket, client_address):
    print(f"[NEW CONNECTION] {client_address} connected.")
    
    # 请求用户名
    client_socket.send(json.dumps({"type": "system", "content": "Please enter your username:"}).encode('utf-8'))
    username = client_socket.recv(1024).decode('utf-8')
    username = json.loads(username)["content"]
    clients[client_socket] = username
    
    # 发送消息历史给新用户
    send_message_history(client_socket)

    # 广播用户上线
    broadcast({"type": "notification", "content": f"{username} has joined the chat!"})
    print(f"[ACTIVE USERS] {list(clients.values())}")
    
    # 主循环，处理消息
    while True:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if not message:
                break

            message_data = json.loads(message)

            if message_data["type"] == "private_message":
                recipient_username = message_data["recipient"]
                broadcast(message_data, current_client_socket=client_socket, recipient=recipient_username)
                save_message(message_data)  # 保存私聊消息
            else:
                broadcast(message_data, current_client_socket=client_socket)
                save_message(message_data)  # 保存群聊消息
        except:
            break

    # 用户离线
    client_socket.close()
    remove_client(client_socket)
    broadcast({"type": "notification", "content": f"{username} has left the chat."})
    print(f"[DISCONNECTED] {client_address} disconnected. [ACTIVE USERS] {list(clients.values())}")

# 启动服务器
def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen()
    print(f"[LISTENING] Server is listening on {HOST}:{PORT}")
    
    while True:
        client_socket, client_address = server.accept()
        thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.active_count() - 1}")

if __name__ == "__main__":
    start_server()
```

### 改进后的客户端代码：

```python
import socket
import threading
import json

HOST = '127.0.0.1'
PORT = 12345

# 接收消息
def receive_messages(client_socket):
    while True:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if message:
                message_data = json.loads(message)
                if message_data["type"] == "notification":
                    print(f"[SYSTEM] {message_data['content']}")
                elif message_data["type"] == "private_message":
                    print(f"[Private from {message_data['username']}] {message_data['content']}")
                elif message_data["type"] == "online_users":
                    print(f"[ONLINE USERS] {', '.join(message_data['content'])}")
                elif message_data["type"] == "history":
                    print(f"[CHAT HISTORY]\n{message_data['content']}")
                else:
                    print(f"[{message_data['username']}] {message_data['content']}")
        except:
            print("An error occurred.")
            client_socket.close()
            break

# 发送消息
def send_messages(client_socket, username):
    while True:
        message = input("")
        if message == "/users":  # 请求查看在线用户
            message_data = {
                "type": "get_users"
            }
            client_socket.send(json.dumps(message_data).encode('utf-8'))
        elif message.startswith("/w"):  # 私聊
            parts = message.split(" ", 2)
            if len(parts) < 3:
                print("Invalid private message format. Use: /w <username> <message>")
            else:
                recipient_username = parts[1]
                content = parts[2]
                message_data = {
                    "type": "private_message",
                    "username": username,
                    "content": content,
                    "recipient": recipient_username
                }
                client_socket.send(json.dumps(message_data).encode('utf-8'))
        else:
            message_data = {
                "type": "message",
                "username": username,
                "content": message
            }
            client_socket.send(json.dumps(message_data).encode('utf-8'))

# 连接服务器
def start_client():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((HOST, PORT))
    
    # 启动接收消息线程
    receive_thread = threading.Thread(target=receive_messages, args=(client,))
    receive_thread.start()
    
    # 输入用户名并发送给服务器
    username = input("Enter your username: ")
    client.send(json.dumps({"type": "username", "content": username}).encode('utf-8'))

    # 启动发送消息线程
    send_thread = threading.Thread(target=send_messages, args=(client, username))
    send_thread.start()

if __name__ == "__main__":
    start_client()
```

### 新功能说明：
1. **消息历史保存**：服务器将每条消息（带时间戳）保存到文件中，文件名为 `chat_history.txt`。
2. **加载历史记录**：当新用户连接时，服务器会将历史消息发送给用户。
3. **时间戳**：每条消息都会附带一个时间戳，以便查看消息的发送时间。

### 下一步建议：
**a.** 将历史记录保存在数据库中（例如 SQLite 或 MySQL），以便更有效地管理和查询。  
**b.** 增加**分页加载历史消息**的功能，防止一次性加载大量历史记录。
