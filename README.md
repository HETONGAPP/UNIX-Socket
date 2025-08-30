# UNIX Socket Communication Example

A simple UNIX domain socket communication program example, implementing a client-server architecture in C++.

## Features

- Stream-based communication using UNIX domain sockets
- Simple command-response protocol
- Support for VERSION command, returns Git commit hash
- Error handling with status codes
- Cross-platform compatibility (Linux/Unix systems)

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

## Docker Build & Usage

### Quick Start

```bash
# Build Docker image
docker build -t unix-socket-app .

# Run container
docker run -it --rm unix-socket-app

# Inside container, test the programs:
cd /app/output/x86_64
./server /tmp/test_socket &
./client /tmp/test_socket VERSION
```

### Supported Architectures

This project supports cross-compilation for multiple architectures:

| Architecture | Compiler | Use Case |
|-------------|----------|----------|
| x86_64 | Native GCC | Development, testing |
| aarch64 | Bootlin Toolchain | ARM64 devices, embedded systems |

### Docker Build Process

The Dockerfile automatically:

1. **Downloads Bootlin aarch64 toolchain** from:
   ```
   https://toolchains.bootlin.com/downloads/releases/toolchains/aarch64/tarballs/aarch64--glibc--stable-2025.08-1.tar.xz
   ```

2. **Compiles for both architectures**:
   - x86_64: Native compilation
   - aarch64: Cross-compilation using downloaded toolchain

3. **Generates executables** in respective directories:
   ```
   output/x86_64/     # Native x86_64 binaries
   output/aarch64/    # Cross-compiled ARM64 binaries
   ```

### Running in Docker

#### For x86_64 Testing (Recommended for development)

```bash
# Build and run
docker build -t unix-socket-app .
docker run -it --rm unix-socket-app

# Inside container:
cd /app/output/x86_64
./server /tmp/test_socket &
./client /tmp/test_socket VERSION
```

#### For aarch64 Binaries

The aarch64 binaries are cross-compiled and should be run on ARM64 hardware:

```bash
# Copy binaries to ARM64 device
docker run --rm unix-socket-app tar czf - output/aarch64 | tar xzf - -C /host/path/

# On ARM64 device:
/path/to/output/aarch64/server /tmp/test_socket &
/path/to/output/aarch64/client /tmp/test_socket VERSION
```

### Architecture Compatibility

⚠️ **Important**: Running aarch64 binaries on x86_64 systems will fail with "Exec format error"

- ✅ x86_64 binaries work on x86_64 systems
- ✅ aarch64 binaries work on ARM64 systems
- ❌ aarch64 binaries do NOT work on x86_64 systems

### Cross-Compilation Details

The project uses Bootlin's pre-built toolchain for aarch64:

- **Toolchain**: aarch64-buildroot-linux-gnu
- **Source**: Bootlin toolchains repository
- **Installation**: `/opt/aarch64-toolchain`
- **Compiler**: `aarch64-buildroot-linux-gnu-g++`

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