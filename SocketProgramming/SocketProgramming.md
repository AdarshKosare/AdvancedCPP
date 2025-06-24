

---

# Socket Programming in C++

Socket programming is a way to enable communication between two nodes (computers, devices, etc.) over a network. Sockets provide an interface for sending and receiving data using standard protocols like TCP and UDP.

## Table of Contents

- [What is a Socket?](#what-is-a-socket)
- [Types of Sockets](#types-of-sockets)
- [Common Use Case](#common-use-case)
- [Socket Programming Workflow](#socket-programming-workflow)
- [Key Socket Functions and Their Functionality](#key-socket-functions-and-their-functionality)
- [Socket Properties](#socket-properties)
- [Example: TCP Client-Server Echo](#example-tcp-client-server-echo)
- [References](#references)

---

## What is a Socket?

A socket is an endpoint for communication between two machines. Sockets can communicate over a network using protocols like TCP (Transmission Control Protocol) or UDP (User Datagram Protocol).

---

## Types of Sockets

- **Stream Sockets (SOCK_STREAM)**: Use TCP, provide reliable, connection-oriented communication.
- **Datagram Sockets (SOCK_DGRAM)**: Use UDP, provide connectionless, unreliable communication.
- **Raw Sockets**: Allow direct sending/receiving of IP packets without protocol-specific transport layer formatting.

---

## Common Use Case

A typical use case is a client-server application:
- **Server**: Listens for incoming connections from clients, processes data, and sends responses.
- **Client**: Connects to the server, sends requests, and receives responses.

Examples: Chat applications, web servers, file transfer, remote command execution.

---

## Socket Programming Workflow

### Server Side

1. **socket()**: Create a new socket.
2. **bind()**: Bind the socket to an IP and port.
3. **listen()**: Listen for incoming connections (TCP only).
4. **accept()**: Accept an incoming connection.
5. **recv()/read()**: Receive data from the client.
6. **send()/write()**: Send data to the client.
7. **close()**: Close the socket.

### Client Side

1. **socket()**: Create a new socket.
2. **connect()**: Connect to the server.
3. **send()/write()**: Send data to the server.
4. **recv()/read()**: Receive data from the server.
5. **close()**: Close the socket.

---

## Key Socket Functions and Their Functionality

| Function           | Usage (Header)         | Description                                                                 |
|--------------------|-----------------------|-----------------------------------------------------------------------------|
| `socket()`         | `<sys/socket.h>`      | Creates a new socket. Returns a file descriptor (int).                      |
| `bind()`           | `<sys/socket.h>`      | Binds socket to a local IP address and port.                                |
| `listen()`         | `<sys/socket.h>`      | Marks socket as passive, to accept incoming connection requests (TCP only). |
| `accept()`         | `<sys/socket.h>`      | Accepts a new connection. Returns a new socket descriptor for communication.|
| `connect()`        | `<sys/socket.h>`      | Initiates a connection to a remote socket (client side).                    |
| `send()`/`write()` | `<sys/socket.h>`      | Sends data over a connected socket.                                         |
| `recv()`/`read()`  | `<sys/socket.h>`      | Receives data from a connected socket.                                      |
| `close()`          | `<unistd.h>`          | Closes the socket file descriptor.                                          |
| `setsockopt()`     | `<sys/socket.h>`      | Sets options on sockets (timeouts, buffer sizes, etc.).                     |
| `getsockopt()`     | `<sys/socket.h>`      | Gets options for sockets.                                                   |

---

### Functionality of Socket Functions

#### 1. `int socket(int domain, int type, int protocol);`
- **domain**: Address family (AF_INET for IPv4, AF_INET6 for IPv6).
- **type**: Socket type (SOCK_STREAM, SOCK_DGRAM).
- **protocol**: Usually set to 0 for default.
- **Returns**: Socket file descriptor.

#### 2. `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`
- Assigns a local protocol address to a socket.

#### 3. `int listen(int sockfd, int backlog);`
- Prepares a bound socket to accept incoming connections; backlog specifies the queue size.

#### 4. `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);`
- Extracts the first connection request and creates a new socket for the connection.

#### 5. `int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`
- Client-side: connects to a remote address.

#### 6. `ssize_t send(int sockfd, const void *buf, size_t len, int flags);`
- Sends data on a connected socket.

#### 7. `ssize_t recv(int sockfd, void *buf, size_t len, int flags);`
- Receives data from a connected socket.

#### 8. `int close(int sockfd);`
- Closes the socket, freeing up resources.

#### 9. `int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);`
- Used to set options like timeouts, buffer sizes.

---

## Socket Properties

- **Blocking/Non-blocking**: Sockets are blocking by default; can be set to non-blocking.
- **Timeouts**: Set with `setsockopt()`, e.g., SO_RCVTIMEO for receive timeout.
- **Port**: Integer value representing the communication endpoint.
- **IP Address**: IPv4 or IPv6 address to which the socket is bound or connected.
- **Buffer Size**: Default system buffer size for send/receive, can be changed.

---

## Example: TCP Client-Server Echo

### Server

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    int addrlen = sizeof(address);
    int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    char buffer[1024] = {0};
    int valread = read(new_socket, buffer, 1024);
    std::cout << "Received: " << buffer << std::endl;
    send(new_socket, buffer, strlen(buffer), 0);

    close(new_socket);
    close(server_fd);
    return 0;
}
```

### Client

```cpp
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    const char* msg = "Hello Server";
    send(sock, msg, strlen(msg), 0);

    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    std::cout << "Echo from server: " << buffer << std::endl;

    close(sock);
    return 0;
}
```

---

## References

- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [Linux man pages: socket(2)](https://man7.org/linux/man-pages/man2/socket.2.html)
- [C++ Reference: <sys/socket.h>](https://en.cppreference.com/w/cpp/header/sys/socket.h)

---

> **Tip:** Always check the return values of socket functions for errors, and use `perror()` or `strerror(errno)` for debugging.

---

## Detailed Explanation: sockaddr_in, sockaddr, and INADDR_* Terms

### sockaddr_in

`sockaddr_in` is a structure used for handling IPv4 addresses in socket programming. It is defined in `<netinet/in.h>` and is used to specify an endpoint address for bind, connect, etc.

```cpp
struct sockaddr_in {
    sa_family_t    sin_family; // Address family (AF_INET for IPv4)
    in_port_t      sin_port;   // Port number (network byte order)
    struct in_addr sin_addr;   // IP address
    char           sin_zero[8];// Padding (unused)
};
```
- **sin_family**: Always set to `AF_INET` for IPv4.
- **sin_port**: The port number (must be in network byte order, use `htons()`).
- **sin_addr**: Structure containing the IP address (in network byte order).
- **sin_zero**: Padding for alignment (should be set to zero).

### sockaddr

`sockaddr` is a generic structure used by system calls like `bind()`, `connect()`, and `accept()`. It is less specific than `sockaddr_in` and is used for type compatibility.

```cpp
struct sockaddr {
    sa_family_t sa_family;    // Address family
    char        sa_data[14];  // Protocol-specific address information
};
```
When calling functions like `bind()` or `connect()`, you cast a `sockaddr_in*` to a `sockaddr*`:
```cpp
bind(sockfd, (struct sockaddr*)&address, sizeof(address));
```

### INADDR_ANY and Other INADDR_* Constants

These macros are defined in `<netinet/in.h>` and are used for special IPv4 addresses in socket programming:

- **INADDR_ANY**:  
  Value: `0.0.0.0`  
  Use: Bind the socket to all available interfaces (any local IP).  
  Example: `address.sin_addr.s_addr = INADDR_ANY;`

- **INADDR_LOOPBACK**:  
  Value: `127.0.0.1`  
  Use: Loopback address for local machine (localhost).  
  Example: `address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);`

- **INADDR_BROADCAST**:  
  Value: `255.255.255.255`  
  Use: Used to send packets to all hosts on the network (broadcast).  
  Example: `address.sin_addr.s_addr = htonl(INADDR_BROADCAST);`

- **INADDR_NONE**:  
  Value: `-1`  
  Use: Indicates an invalid address, often returned by functions like `inet_addr()` on error.

#### Example Usage Table

| Macro               | Typical Usage                                      | Value             |
|---------------------|----------------------------------------------------|-------------------|
| `INADDR_ANY`        | Bind to all interfaces                             | `0.0.0.0`         |
| `INADDR_LOOPBACK`   | Loopback interface (localhost)                     | `127.0.0.1`       |
| `INADDR_BROADCAST`  | Broadcast to all hosts                             | `255.255.255.255` |
| `INADDR_NONE`       | Error/invalid address indication                   | `-1`              |

#### Typical sockaddr_in Initialization

```cpp
sockaddr_in address;
address.sin_family = AF_INET;
address.sin_port = htons(8080);
// Bind to all available interfaces
address.sin_addr.s_addr = INADDR_ANY;
// Or bind to localhost only
address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
// Or set a specific IP
inet_pton(AF_INET, "192.168.1.10", &address.sin_addr);
memset(address.sin_zero, 0, sizeof(address.sin_zero));
```

---

**Tip:** Always use `htonl()` or `htons()` to convert values to network byte order when assigning to `sin_addr.s_addr` or `sin_port` respectively, except when using `inet_pton()` or `inet_addr()` which already return the address in the correct format.

---

You can copy and add this section after the "Socket Properties" section (before the Example section) in your `SocketProgramming.md` for a more comprehensive resource. Let me know if you want this inserted directly into your current file content!

You can copy and save this content as `SocketProgramming.md` in your repository. Let me know if you want a more advanced example or details on UDP sockets!