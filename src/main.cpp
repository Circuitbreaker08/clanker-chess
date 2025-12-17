#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <unordered_map>
#include <exception>

#define CROW_STATIC_DIRECTORY "web/"
#define CROW_STATIC_ENDPOINT "/<path>"

#include "crow.h"

#include "file.hpp"

void print_headers(const crow::request& req) {
    for (const auto& header : req.headers) {
        std::cout << header.first << ": " << header.second << std::endl;
    }
}

crow::json::rvalue default_board;

Tracker token_registry {};
Tracker counter {};

std::unordered_map<int, std::shared_ptr<Tracker>> user_files;

int main() {
    // Set cwd to executable location
    std::filesystem::current_path(std::filesystem::canonical("/proc/self/exe").parent_path());

    token_registry = Tracker("data/token_registry.json");
    counter = Tracker("data/counter.json", [](crow::json::wvalue& json){
        json["id"] = 0;
        json["token"] = 0;
        json["game"] = 0;
    });

    if (!std::filesystem::exists("data/users")) {
        std::filesystem::create_directory("data/users");
    }

    for (const auto& file : std::filesystem::directory_iterator("data/users/")) {
        std::cout << "Loading player file at " + file.path().string() + "\n";
        user_files[
            std::stoi(
                file.path().filename().string().substr(0, file.path().filename().string().find("."))
            )
        ] = std::make_shared<Tracker>(Tracker(file.path().string()));
    }

    default_board = load_json("data/default_board.json");

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](const crow::request &req, crow::response &res){
        res.redirect("./index.html");
        res.end();
    });

    CROW_ROUTE(app, "/api/token_validator").methods("POST"_method)
    ([](const crow::request& req) {
        const std::string token = req.get_header_value("token");
        token_registry.mutex->lock_shared();
        for (auto& key : token_registry.json.keys()) {
            if (token == key) {
                token_registry.mutex->unlock_shared();
                return "{\"valid\": true}";
            }
        }
        token_registry.mutex->unlock_shared();
        return "{\"valid\": false}";
    });

    CROW_ROUTE(app, "/api/token_to_uuid").methods("POST"_method)
    ([](const crow::request& req) {
        const std::string token = req.get_header_value("token");
        token_registry.mutex->lock_shared();
        for (auto& key : token_registry.json.keys()) {
            if (token == key) {
                std::string res = std::string("{\"uuid\": ") + token_registry.json[token].dump() + "}";
                token_registry.mutex->unlock_shared();
                return res;
            }
        }
        token_registry.mutex->unlock_shared();
        return std::string("{\"uuid\": null}");
    });

    CROW_ROUTE(app, "/api/uuid_lookup/<int>").methods("GET"_method)
    ([](int uuid) {
        return uuid;
    });

    app.port(8765).multithreaded().run();
}