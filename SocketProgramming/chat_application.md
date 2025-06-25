# Multi-Client Chat Application in C++

This project demonstrates a simple multi-client chat application using socket programming in C++. The application includes a server and client implementation. The server can handle multiple clients simultaneously and broadcast messages to all connected clients.

## Features
- Server-side multi-client handling using threads.
- Message broadcasting to all connected clients.
- Clean handling of client disconnections.

---

## Server Code

### **Explanation**
The server:
1. Listens for incoming connections.
2. Maintains a list of all connected clients.
3. Broadcasts messages from one client to all others.
4. Handles client disconnections gracefully.

### **Code**
```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

std::vector<int> client_sockets;
std::mutex clients_mutex;

void broadcast_message(const std::string& message, int sender_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int client_socket : client_sockets) {
        if (client_socket != sender_socket) {
            send(client_socket, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    while (true) {
        int bytes_read = read(client_socket, buffer, 1024);
        if (bytes_read <= 0) {
            std::lock_guard<std::mutex> lock(clients_mutex);
            client_sockets.erase(
                std::remove(client_sockets.begin(), client_sockets.end(), client_socket),
                client_sockets.end()
            );
            std::cout << "Client disconnected.\n";
            close(client_socket);
            break;
        }
        std::string message(buffer, bytes_read);
        std::cout << "Message received: " << message << std::endl;
        broadcast_message(message, client_socket);
        std::memset(buffer, 0, sizeof(buffer));
    }
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on port 8080\n";

    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            client_sockets.push_back(client_socket);
        }

        std::cout << "New client connected\n";
        std::thread(handle_client, client_socket).detach();
    }

    close(server_fd);
    return 0;
}
```

---

## Client Code

### **Explanation**
The client:
1. Connects to the server.
2. Continuously sends messages typed by the user.
3. Listens for broadcast messages from the server in a separate thread.

### **Code**
```cpp
#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void receive_messages(int client_socket) {
    char buffer[1024];
    while (true) {
        int bytes_read = read(client_socket, buffer, 1024);
        if (bytes_read <= 0) {
            std::cout << "Disconnected from server.\n";
            break;
        }
        std::cout << "\rMessage from server: " << std::string(buffer, bytes_read) << "\n> ";
        std::cout.flush();
        std::memset(buffer, 0, sizeof(buffer));
    }
}

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in server_address = {};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return 1;
    }

    std::cout << "Connected to server. Type your messages:\n";

    std::thread receiver(receive_messages, client_socket);
    while (true) {
        std::string message;
        std::getline(std::cin, message);
        if (message == "/exit") {
            std::cout << "Exiting...\n";
            break;
        }
        send(client_socket, message.c_str(), message.size(), 0);
    }

    close(client_socket);
    receiver.join();
    return 0;
}
```

---

## Steps to Run

### **1. Compile the Code**
Use `g++` to compile the server and client codes separately:
```bash
# Compile server
g++ server.cpp -o server -pthread

# Compile client
g++ client.cpp -o client -pthread
```

### **2. Start the Server**
Run the server executable:
```bash
./server
```

### **3. Start Clients**
Run multiple instances of the client executable:
```bash
./client
```

### **4. Test the Application**
- Type messages in one client window.
- Observe them being broadcasted to other connected clients.

---

## Future Enhancements
- **Private Messaging:** Allow users to send private messages to specific clients.
- **User Authentication:** Add login and registration functionality.
- **Graphical User Interface (GUI):** Build a simple GUI using libraries like Qt or GTK.
- **Encryption:** Use SSL/TLS for secure communication.
- **File Transfer:** Add support for sending files between clients.
