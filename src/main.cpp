#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

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

Tracker token_registry;
Tracker counter;
Tracker taken_usernames;

std::unordered_map<int, std::shared_ptr<Tracker>> user_files;

std::vector<std::shared_ptr<int>> active_games;

bool check_admin_password(std::string password) {
    return password == read_file("admin");
}

int main() {
    // Set cwd to executable location
    std::filesystem::current_path(std::filesystem::canonical("/proc/self/exe").parent_path());

    std::filesystem::create_directory("data");

    token_registry = Tracker("data/token_registry.json");
    counter = Tracker("data/counter.json", [](crow::json::wvalue& json){
        json["id"] = 0;
        json["token"] = 0;
        json["game"] = 0;
    });
    taken_usernames = Tracker("data/taken_usernames.json", [](crow::json::wvalue& json){
        json = crow::json::wvalue::list();
    });
    
    std::filesystem::create_directory("data/users");
    for (const auto& file : std::filesystem::directory_iterator("data/users/")) {
        user_files[
            std::stoi(
                file.path().filename().string().substr(0, file.path().filename().string().find("."))
            )
        ] = std::make_shared<Tracker>(Tracker(file.path().string()));
    }

    std::filesystem::create_directory("data/games");

    default_board = load_json("default_board.json");

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
        crow::json::wvalue res;
        {
            auto it = user_files.find(uuid);
            if (it == user_files.end()) {
                res["ok"] = false;
            } else {
                user_files[uuid]->mutex->lock_shared();
                res = crow::json::load(user_files[uuid]->json.dump());
                user_files[uuid]->mutex->unlock_shared();
                res["ok"] = true;
            }
        }
        return res;
    });

    CROW_ROUTE(app, "/api/game/count").methods("GET"_method)
    ([]() {
        int filecount = 0;
        for (const auto& file : std::filesystem::directory_iterator("data/games/")) {
            filecount++;
        }
        crow::json::wvalue out = crow::json::wvalue();
        return "{active:" + std::to_string(active_games.size()) + ", completed: " + std::to_string(filecount) + "}";
    });

    CROW_ROUTE(app, "/api/game/<int>").methods("GET"_method)
    ([](int id) {
        return id;
    });

    CROW_ROUTE(app, "/api/admin/new_user").methods("POST"_method)
    ([](const crow::request& req) {
        std::string username = req.get_header_value("username");
        taken_usernames.mutex->lock();
        
        if(check_admin_password(req.get_header_value("token"))) {
            for (int i = 0; i < taken_usernames.json.size(); i++) {
                if (taken_usernames.json[i].dump() == username) {
                    taken_usernames.mutex->unlock();
                    return false;
                }
            }
            taken_usernames.json += "";
            taken_usernames.mutex->unlock();
            return true;
        } else {
            taken_usernames.mutex->unlock();
            return false;
        }
    });

    app.port(8765).multithreaded().run();
}