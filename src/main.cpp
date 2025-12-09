#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

#define CROW_STATIC_DIRECTORY "web/"
#define CROW_STATIC_ENDPOINT "/<path>"

#include "crow.h"

#include "file.hpp"

void print_headers(const crow::request req) {
    for (const auto& header : req.headers) {
        std::cout << header.first << ": " << header.second << std::endl;
    }
}

crow::json::rvalue default_board;

Tracker token_registry {"data/token_registry.json"};
Tracker counter {"data/counter.json", [](crow::json::wvalue& json){
    json["id"] = 0;
    json["token"] = 0;
    json["game"] = 0;
}};

int main() {
    // Set cwd to executable location
    std::filesystem::current_path(std::filesystem::canonical("/proc/self/exe").parent_path());

    Tracker::init();

    default_board = load_json("data/default_board.json");

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](const crow::request &req, crow::response &res){
        res.redirect("./index.html");
        res.end();
    });

    CROW_ROUTE(app, "/api/token_validator").methods("POST"_method)
    ([](const crow::request& req) {
        const std::string token = req.get_header_value("token");
        for (auto& key : token_registry.json.keys()) {
            if (token == key) {
                return "{\"valid\": true}";
            }
        }
        return "{\"valid\": false}";
    });

    app.port(8765).multithreaded().run();
}