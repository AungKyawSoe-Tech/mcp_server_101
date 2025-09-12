#include "http_server.h"
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void start_http_server(int port) {
    std::thread([port]() {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        bind(server_fd, (sockaddr*)&addr, sizeof(addr));
        listen(server_fd, 5);
        std::cout << "[HTTP] Listening on port " << port << std::endl;
        while (true) {
            int client_fd = accept(server_fd, nullptr, nullptr);
            if (client_fd >= 0) {
                char buf[1024] = {0};
                read(client_fd, buf, sizeof(buf));
                // Minimal HTTP response
                const char* resp = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from HTTP server!";
                write(client_fd, resp, strlen(resp));
                close(client_fd);
            }
        }
        close(server_fd);
    }).detach();
}