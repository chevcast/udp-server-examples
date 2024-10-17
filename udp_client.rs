// UDP Client in Rust
// Sends a message to UDP ports 8080, 8081, 8082, and 8083 on localhost
//
// Usage:
//     ./udp_client "Your message here"
//
// How to compile:
//     rustc -o udp_client_rs udp_client.rs
//
// How to run:
//     ./udp_client_rs "Hello, World!"

use std::env;
use std::net::UdpSocket;

fn main() {
    // Collect command-line arguments
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        eprintln!("Usage: {} \"Your message here\"", args[0]);
        std::process::exit(1);
    }

    let message = args[1].as_bytes();

    // Ports to send the message to
    let ports = [8080, 8081, 8082, 8083];

    // Create a UDP socket
    let socket = UdpSocket::bind("0.0.0.0:0").expect("Could not bind socket");

    // Send the message to each port
    for port in &ports {
        let addr = format!("127.0.0.1:{}", port);
        match socket.send_to(message, &addr) {
            Ok(_) => println!("Message sent to port {}", port),
            Err(e) => eprintln!("Failed to send to port {}: {}", port, e),
        }
    }

    // Socket will be closed when it goes out of scope
}
