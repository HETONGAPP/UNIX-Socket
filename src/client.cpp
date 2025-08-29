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
        return 1;
    }

    const char *socket_path = argv[1];
    const char *command = argv[2];

    int sock_fd;
    struct sockaddr_un addr;

    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 2;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    if (connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return 3;
    }

    std::string cmd = std::string(command) + "\n";
    if (write(sock_fd, cmd.c_str(), cmd.size()) < 0) {
        perror("write command");
        close(sock_fd);
        return 4;
    }

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    int n = read(sock_fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        std::cout << buffer << std::endl;
    }

    close(sock_fd);
    return 0;
}
