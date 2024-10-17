/*
 * UDP Server in C++
 * Listens on port 8081
 * Prints "C++ server received: <message>" when a message is received
 * Handles Ctrl+C gracefully
 *
 * How to compile:
 *     g++ -o udp_server_cpp udp_server.cpp
 *
 * How to run:
 *     ./udp_server_cpp
 *
 * Press Ctrl+C to stop the server.
 */

#include <arpa/inet.h> // For sockaddr_in and inet_ntoa()
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h> // For socket functions
#include <unistd.h>     // For close()

int sockfd; // Socket file descriptor

void handle_sigint(int sig) {
  // Close the socket and exit
  close(sockfd);
  std::cout << "\nServer terminated gracefully" << std::endl;
  exit(0);
}

int main() {
  struct sockaddr_in server_addr, client_addr;
  char buffer[1024];
  socklen_t addr_len = sizeof(client_addr);

  // Set up the signal handler for Ctrl+C (SIGINT)
  signal(SIGINT, handle_sigint);

  // Create a UDP socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Zero out the server address structure
  memset(&server_addr, 0, sizeof(server_addr));

  // Set up the server address
  server_addr.sin_family = AF_INET;         // IPv4
  server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces
  server_addr.sin_port = htons(8081);       // Port 8081

  // Bind the socket to the server address
  if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Bind failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  std::cout << "C++ UDP server listening on port 8081" << std::endl;

  while (true) {
    // Receive a message from a client
    ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&client_addr, &addr_len);
    if (n < 0) {
      perror("Receive failed");
      continue;
    }

    buffer[n] = '\0'; // Null-terminate the received data

    // Print the received message
    std::cout << "C++ server received: " << buffer << std::endl;
  }

  // Close the socket (unreachable code in this example)
  close(sockfd);

  return 0;
}
