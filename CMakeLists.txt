cmake_minimum_required(VERSION 3.10)
project(mcp_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(mcp_server
    src/main.cpp
    src/config.cpp
    src/tcp_server.cpp
    src/http_server.cpp
    src/coap_server.cpp
)

# For JSON parsing (nlohmann/json)
include(FetchContent)
FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)
target_link_libraries(mcp_server PRIVATE nlohmann_json::nlohmann_json)

find_package(CURL REQUIRED)
target_link_libraries(mcp_server PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)