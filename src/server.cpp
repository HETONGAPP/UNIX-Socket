#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdlib>

#ifndef VERSION_HASH
#define VERSION_HASH "unknown"
#endif

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <socket_path>" << std::endl;
        return 1; // parameter error
    }

    const char *socket_path = argv[1];
    int server_fd, client_fd;
    struct sockaddr_un addr;

    // create socket
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error: failed to create socket" << std::endl;
        perror("socket");
        return 2;
    }

    // delete existing socket file (if exists)
    unlink(socket_path);

    // fill address structure
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    // bind socket
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error: failed to bind socket " << socket_path << std::endl;
        perror("bind");
        return 3;
    }

    // listen
    if (listen(server_fd, 5) < 0) {
        std::cerr << "Error: failed to listen on socket " << socket_path << std::endl;
        perror("listen");
        return 4;
    }

    std::cout << "Server listening on " << socket_path << std::endl;

    while (true) {
        // accept client connection
        if ((client_fd = accept(server_fd, nullptr, nullptr)) < 0) {
            std::cerr << "Warning: failed to accept client connection" << std::endl;
            perror("accept");
            continue; // continue waiting for next client
        }

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));

        // read client command
        int n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            std::string cmd(buffer);

            if (cmd == "VERSION\n" || cmd == "VERSION") {
                std::string version = VERSION_HASH;
                if (write(client_fd, version.c_str(), version.size()) < 0) {
                    std::cerr << "Warning: failed to write VERSION to client" << std::endl;
                    perror("write version");
                }
            } else {
                if (write(client_fd, "REJECTED", 8) < 0) {
                    std::cerr << "Warning: failed to write REJECTED to client" << std::endl;
                    perror("write rejected");
                }
            }
        } else if (n < 0) {
            std::cerr << "Warning: failed to read from client" << std::endl;
            perror("read");
        }

        close(client_fd); // close client connection
    }

    // clean up
    close(server_fd);
    unlink(socket_path);
    return 0;
}
