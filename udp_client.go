/*
 * UDP Client in Go
 * Sends a message to UDP ports 8080, 8081, 8082, and 8083 on localhost
 *
 * Usage:
 *     ./udp_client_go "Your message here"
 *
 * How to compile:
 *     go build -o udp_client_go udp_client.go
 *
 * How to run:
 *     ./udp_client_go "Hello, World!"
 */

package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Printf("Usage: %s \"Your message here\"\n", os.Args[0])
		os.Exit(1)
	}

	message := os.Args[1]

	// Ports to send the message to
	ports := []int{8080, 8081, 8082, 8083}

	// Send the message to each port
	for _, port := range ports {
		addr := fmt.Sprintf("127.0.0.1:%d", port)
		conn, err := net.Dial("udp", addr)
		if err != nil {
			fmt.Printf("Failed to connect to %s: %v\n", addr, err)
			continue
		}

		_, err = conn.Write([]byte(message))
		if err != nil {
			fmt.Printf("Failed to send to %s: %v\n", addr, err)
		} else {
			fmt.Printf("Message sent to port %d\n", port)
		}

		conn.Close()
	}
}
