# UNIX Socket Communication Example

A simple UNIX domain socket communication program example, implementing a client-server architecture in C++.

## Features

- Stream-based communication using UNIX domain sockets
- Simple command-response protocol
- Support for VERSION command, returns Git commit hash
- Error handling with status codes
- Cross-platform compatibility (Linux/Unix systems)

## Project Structure

```
UNIX-Socket/
├── makefile          # Build script
├── README.md         # Project documentation
├── src/
│   ├── server.cpp    # Server program
│   └── client.cpp    # Client program
└── output/           # Build output directory
    ├── server        # Server executable
    └── client        # Client executable
```

## Requirements

- **Compiler**: GCC 4.8+ or compatible C++ compiler
- **System**: Linux or other Unix-like systems supporting UNIX domain sockets
- **Git**: For version information (optional)

## Build Instructions

### Using Make

```bash
# Build all targets
make all

# Build server only
make output/server

# Build client only
make output/client

# Clean build artifacts
make clean
```

### Manual Compilation

```bash
# Create output directory
mkdir -p output

# Compile server (with version info)
g++ -Wall -O2 -DVERSION_HASH="$(git rev-parse HEAD)" -o output/server src/server.cpp

# Compile client
g++ -Wall -O2 -o output/client src/client.cpp
```

## Usage

### Starting the Server

```bash
./output/server <socket_path>
```

**Parameters:**
- `socket_path`: UNIX socket file path, e.g., `/tmp/my_socket`

**Example:**
```bash
./output/server /tmp/test_socket
```

The server will display listening status and wait for client connections.

### Running the Client

```bash
./output/client <socket_path> <command>
```

**Parameters:**
- `socket_path`: Same socket path as the server
- `command`: Command to send to the server

**Examples:**

```bash
# Query version information
./output/client /tmp/test_socket VERSION

# Send other commands (will be rejected by server)
./output/client /tmp/test_socket HELLO
```

## Supported Commands

### VERSION
Query server version information, returns Git commit hash.

**Request:**
```
VERSION
```

**Response:**
```
fcc144bdc4ac778a982f44c4fd8e40561f1537b0
```

### Other Commands
The server will reject all unsupported commands with a fixed response.

**Request:**
```
Any other command
```

**Response:**
```
REJECTED
```