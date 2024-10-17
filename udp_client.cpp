/*
 * UDP Client in C++
 * Sends a message to UDP ports 8080, 8081, 8082, and 8083 on localhost
 *
 * Usage:
 *     ./udp_client_cpp "Your message here"
 *
 * How to compile:
 *     g++ -o udp_client_cpp udp_client.cpp
 *
 * How to run:
 *     ./udp_client_cpp "Hello, World!"
 */

#include <arpa/inet.h> // For sockaddr_in and inet_pton()
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h> // For socket functions
#include <unistd.h>     // For close()

int main(int argc, char *argv[]) {
  int sockfd;
  struct sockaddr_in server_addr;
  std::string message;

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " \"Your message here\"" << std::endl;
    exit(EXIT_FAILURE);
  }

  message = argv[1];

  // Ports to send the message to
  int ports[] = {8080, 8081, 8082, 8083};
  int num_ports = sizeof(ports) / sizeof(ports[0]);

  // Create a UDP socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Zero out the server address structure
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET; // IPv4

  // Convert localhost IP address
  if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
    perror("Invalid address");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // Send the message to each port
  for (int i = 0; i < num_ports; i++) {
    server_addr.sin_port = htons(ports[i]); // Set the port

    // Send the message
    ssize_t n =
        sendto(sockfd, message.c_str(), message.length(), 0,
               (const struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0) {
      perror("Send failed");
      continue;
    }

    std::cout << "Message sent to port " << ports[i] << std::endl;
  }

  // Close the socket
  close(sockfd);

  return 0;
}
