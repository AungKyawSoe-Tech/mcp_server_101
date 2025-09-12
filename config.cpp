#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>

bool load_config(const std::string& filename, ServerConfig& config) {
    std::ifstream f(filename);
    if (!f.is_open()) return false;
    nlohmann::json j;
    f >> j;
    config.tcp_port = j.value("tcp_port", 5000);
    config.http_port = j.value("http_port", 8080);
    config.coap_port = j.value("coap_port", 5683);
    return true;
}