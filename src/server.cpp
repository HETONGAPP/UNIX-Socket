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
        return 1;
    }

    const char *socket_path = argv[1];
    int server_fd, client_fd;
    struct sockaddr_un addr;

    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 2;
    }

    unlink(socket_path);

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 3;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        return 4;
    }

    std::cout << "Server listening on " << socket_path << std::endl;

    while (true) {
        if ((client_fd = accept(server_fd, nullptr, nullptr)) < 0) {
            perror("accept");
            continue;
        }

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        int n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            std::string cmd(buffer);
            if (cmd == "VERSION\n" || cmd == "VERSION") {
                std::string version = VERSION_HASH;
                if (write(client_fd, version.c_str(), version.size()) < 0) {
                    perror("write version");
                }
            } else {
                if (write(client_fd, "REJECTED", 8) < 0) {
                    perror("write rejected");
                }
            }
        }
        close(client_fd);
    }

    close(server_fd);
    unlink(socket_path);
    return 0;
}
