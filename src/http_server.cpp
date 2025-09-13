#include "http_server.h"
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <chrono>
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <string>
#include <fstream>

// Helper for libcurl response
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to call OpenAI API
std::string call_llm_api(const std::string& prompt, const std::string& api_key) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    if(curl) {
        nlohmann::json req = {
            {"model", "gpt-3.5-turbo"},
            {"messages", {{{"role", "user"}, {"content", prompt}}}}
        };
        struct curl_slist* headers = nullptr;
        std::string auth = "Authorization: Bearer " + api_key;
        headers = curl_slist_append(headers, auth.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.dump().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    // Parse response
    try {
        auto resp = nlohmann::json::parse(readBuffer);
        return resp["choices"][0]["message"]["content"];
    } catch (...) {
        return "LLM error or invalid response";
    }
}

void start_http_server(int port, const std::string& openai_api_key) {
    httplib::Server svr;

    svr.Post("/llm", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto j = nlohmann::json::parse(req.body);
            std::string prompt = j["prompt"];
            std::string llm_response = call_llm_api(prompt, openai_api_key);
            nlohmann::json resp = {{"response", llm_response}};
            res.set_content(resp.dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content("{\"error\":\"Invalid request\"}", "application/json");
        }
    });

    // ...existing endpoints...

    svr.listen("0.0.0.0", port);
}