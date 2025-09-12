#include "tcp_server.h"
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void start_tcp_server(int port) {
    std::thread([port]() {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        bind(server_fd, (sockaddr*)&addr, sizeof(addr));
        listen(server_fd, 5);
        std::cout << "[TCP] Listening on port " << port << std::endl;
        while (true) {
            int client_fd = accept(server_fd, nullptr, nullptr);
            if (client_fd >= 0) {
                std::cout << "[TCP] Client connected" << std::endl;
                // Echo example
                char buf[1024];
                ssize_t n = read(client_fd, buf, sizeof(buf));
                write(client_fd, buf, n);
                close(client_fd);
            }
        }
        close(server_fd);
    }).detach();
}