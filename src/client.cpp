#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <socket_path> <command>" << std::endl;
        return 1; // parameter error
    }

    const char *socket_path = argv[1];
    const char *command = argv[2];

    int sock_fd;
    struct sockaddr_un addr;

    // create socket
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error: failed to create socket" << std::endl;
        perror("socket");
        return 2;
    }

    // fill address structure
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    // connect to server
    if (connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error: failed to connect to socket " << socket_path << std::endl;
        perror("connect");
        close(sock_fd);
        return 3; // connection failed exit positive
    }

    // send command
    std::string cmd = std::string(command) + "\n";
    if (write(sock_fd, cmd.c_str(), cmd.size()) < 0) {
        std::cerr << "Error: failed to send command to server" << std::endl;
        perror("write command");
        close(sock_fd);
        return 4; // write failed exit positive
    }

    // read response
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    int n = read(sock_fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        std::cout << buffer << std::endl; // output to stdout
    } else if (n < 0) {
        std::cerr << "Warning: failed to read response from server" << std::endl;
        perror("read");
    }

    close(sock_fd);
    return 0;
}
