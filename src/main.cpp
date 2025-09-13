#include "config.h"
#include "tcp_server.h"
#include "http_server.h"
#include "coap_server.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <string>

int main() {
    ServerConfig config;
    if (!load_config("config.json", config)) {
        std::cerr << "Failed to load config.json" << std::endl;
        return 1;
    }
    start_tcp_server(config.tcp_port);

    std::string openai_api_key;
    std::ifstream keyfile("openai_api_key.txt");
    if (keyfile) std::getline(keyfile, openai_api_key);

    // Pass the key to your HTTP server
    start_http_server(8080, openai_api_key);
    start_coap_server(config.coap_port);

    std::cout << "MCP server running. Press Ctrl+C to exit." << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }
    return 0;
}