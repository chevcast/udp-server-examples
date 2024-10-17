// UDP Server in Rust
// Listens on port 8082
// Prints "Rust server received: <message>" when a message is received
// Handles Ctrl+C gracefully
//
// How to compile:
//     rustc -o udp_server_rs udp_server.rs
//
// How to run:
//     ./udp_server_rs
//
// Press Ctrl+C to stop the server.

use std::io;
use std::net::UdpSocket;
use std::process::exit;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

// Import required types for FFI
use std::os::raw::{c_int, c_void};

// Signal constants (platform-specific)
const SIGINT: c_int = 2; // Interrupt signal

// Declare the external C function for signal handling
extern "C" {
    fn signal(sig: c_int, handler: extern "C" fn(c_int)) -> c_void;
}

// Atomic flag to control the running state
static RUNNING: AtomicBool = AtomicBool::new(true);

// Signal handler function
extern "C" fn handle_signal(_: c_int) {
    // Handle the signal (e.g., Ctrl+C)
    RUNNING.store(false, Ordering::SeqCst);
    println!("\nServer terminated gracefully");
    // Exit the process
    exit(0);
}

fn main() -> io::Result<()> {
    // Set up the signal handler for SIGINT (Ctrl+C)
    unsafe {
        signal(SIGINT, handle_signal);
    }

    // Bind the UDP socket to port 8082
    let socket = UdpSocket::bind("127.0.0.1:8082")?;
    println!("Rust UDP server listening on port 8082");

    // Set the socket to non-blocking mode
    socket.set_nonblocking(true)?;

    let mut buf = [0u8; 1024];

    while RUNNING.load(Ordering::SeqCst) {
        match socket.recv_from(&mut buf) {
            Ok((amt, _)) => {
                let msg = String::from_utf8_lossy(&buf[..amt]);
                println!("Rust server received: {}", msg);
            }
            Err(ref e) if e.kind() == io::ErrorKind::WouldBlock => {
                // No data available yet
                thread::sleep(std::time::Duration::from_millis(100));
                continue;
            }
            Err(e) => {
                eprintln!("Error receiving data: {}", e);
                break;
            }
        }
    }

    // Socket will be closed when it goes out of scope
    Ok(())
}
