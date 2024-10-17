/*
 * UDP Server in Go
 * Listens on port 8083
 * Prints "Go server received: <message>" when a message is received
 * Handles Ctrl+C gracefully
 *
 * How to compile:
 *     go build -o udp_server_go udp_server.go
 *
 * How to run:
 *     ./udp_server_go
 *
 * Press Ctrl+C to stop the server.
 */

package main

import (
	"fmt"
	"net"
	"os"
	"os/signal"
	"syscall"
	"time"
)

func main() {
	// Create a UDP address to listen on
	addr := net.UDPAddr{
		Port: 8083,
		IP:   net.ParseIP("127.0.0.1"),
	}

	// Listen for incoming UDP packets
	conn, err := net.ListenUDP("udp", &addr)
	if err != nil {
		fmt.Println("Error starting server:", err)
		return
	}
	defer conn.Close()
	fmt.Println("Go UDP server listening on port 8083")

	// Channel to handle OS signals
	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)

	// Channel to signal the server to stop
	done := make(chan bool, 1)

	// Goroutine to handle incoming messages
	go func() {
		buf := make([]byte, 1024)
		for {
			select {
			case <-done:
				return
			default:
				conn.SetReadDeadline(time.Now().Add(1 * time.Second))
				n, _, err := conn.ReadFromUDP(buf)
				if err != nil {
					if netErr, ok := err.(net.Error); ok && netErr.Timeout() {
						continue
					}
					fmt.Println("Error reading:", err)
					continue
				}
				message := string(buf[:n])
				fmt.Printf("Go server received: %s\n", message)
			}
		}
	}()

	// Wait for Ctrl+C (SIGINT)
	<-sigs
	fmt.Println("\nServer terminated gracefully")
	close(done)
}
