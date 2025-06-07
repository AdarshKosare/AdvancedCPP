#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

using namespace std;

int main()
{
   int server_fd, new_socket;
   struct sockaddr_in address;
   int addrlen = sizeof(address);
   char buffer[1024] = {0};

   // 1. Create Socket
   server_fd = socket(AF_INET, SOCK_STREAM, 0);

   if (server_fd < 0)
   {
      perror("Socket Creation Failed");
      exit(EXIT_FAILURE);
   }

   // 2. Setup the address structure

   address.sin_family = AF_INET;         // IPV4
   address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
   address.sin_port = htons(PORT);       // Port Number

   // 3. Bind the socket to the specified IP and port

   if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
   {
      perror("Binde is Failed");
      exit(EXIT_FAILURE);
   }

   // 4. Listen for incoming connections (max 3 in queue)

   if (listen(server_fd, 3) < 0)
   {
      perror("Listen is failed");
      exit(EXIT_FAILURE);
   }

   cout << "Serverlistening on port " << PORT << endl;

   // 5. Accept a connection from a client

   new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
   if (new_socket < 0)
   {
      perror("Accept is failed");
      exit(EXIT_FAILURE);
   }

   cout << "Client Conneted !" << endl;

   // 6. Chat loop : receive and send messages
   while (true)
   {
      memset(buffer, 0, sizeof(buffer));            // Clear Buffer
      int valread = read(new_socket, buffer, 1024); //  Read message form client
      if (valread <= 0)
         break; // Exit if client disconnects

      cout << "Client:" << buffer << endl;

      cout << "Server: ";
      string reply;
      getline(cin, reply);                                // Get Sever reply from cmd
      send(new_socket, reply.c_str(), reply.length(), 0); // Send reply to client
      if (reply == "exit")
         break;
   }

   close(new_socket);
   close(server_fd);
   return 0;
}
