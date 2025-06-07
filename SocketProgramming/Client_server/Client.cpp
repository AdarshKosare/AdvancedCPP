#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // 1. Create a socket (IPv4, TCP)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    // 2. Set up the server address structure
    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_port = htons(PORT); // Port number

    // 3. Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address" << std::endl;
        return -1;
    }

    // 4. Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    // 5. Chat loop: send and receive messages
    while (true) {
        std::cout << "Client: ";
        std::string msg;
        std::getline(std::cin, msg); // Get client message from console
        send(sock, msg.c_str(), msg.length(), 0); // Send message to server
        if (msg == "exit") break; // Exit if client types "exit"

        memset(buffer, 0, sizeof(buffer)); // Clear buffer
        int valread = read(sock, buffer, 1024); // Read server reply
        if (valread <= 0) break; // Exit if server disconnects
        std::cout << "Server: " << buffer << std::endl;
    }

    // 6. Close the socket
    close(sock);
    return 0;
}