#include "coap_server.h"
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void start_coap_server(int port) {
    std::thread([port]() {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        bind(sockfd, (sockaddr*)&addr, sizeof(addr));
        std::cout << "[CoAP] Listening on UDP port " << port << std::endl;
        while (true) {
            char buf[1024] = {0};
            sockaddr_in client_addr{};
            socklen_t len = sizeof(client_addr);
            ssize_t n = recvfrom(sockfd, buf, sizeof(buf), 0, (sockaddr*)&client_addr, &len);
            if (n > 0) {
                std::cout << "[CoAP] Received UDP packet" << std::endl;
                // Echo back
                sendto(sockfd, buf, n, 0, (sockaddr*)&client_addr, len);
            }
        }
        close(sockfd);
    }).detach();
}