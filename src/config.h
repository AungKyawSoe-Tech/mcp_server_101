#pragma once
#include <string>

struct ServerConfig {
    int tcp_port;
    int http_port;
    int coap_port;
};

bool load_config(const std::string& filename, ServerConfig& config);