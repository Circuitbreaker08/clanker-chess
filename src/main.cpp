#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

#define CROW_STATIC_DIRECTORY "web/"
#define CROW_STATIC_ENDPOINT "/<path>"

#include "crow.h"

#include "file.hpp"

crow::json::rvalue default_board;

int main() {
    // Set cwd to file location
    std::filesystem::current_path(std::filesystem::canonical("/proc/self/exe").parent_path());

    default_board = load_json("data/default_board.json");

    // Ensure files are ready
    // TODO - start using Tracker class
    crow::json::wvalue counter;
    if (!std::filesystem::exists("data/counter.json")) {
        counter["id"] = 0;
        counter["token"] = 0;
        counter["game"] = 0;
        write_file("data/counter.json", counter.dump());
    } else {
        counter = load_json("data/counter.json");
    }

    crow::json::wvalue token_registry;
    if (!std::filesystem::exists("/data/token_registry.json")) {
        
    } else {
        token_registry = load_json("data/token_registry.json");
    }

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](const crow::request &req, crow::response &res){
        res.redirect("./index.html");
        res.end();
    });

    CROW_ROUTE(app, "/api/token_validator").methods("POST"_method)
    ([](const crow::request&, crow::response& res) {
        //replace cat.jpg with your file path
        res.set_static_file_info("cat.jpg");
        res.end();

        // TODO - check if token valid and add a call to the homepage
    });

    app.port(8765).multithreaded().run();
}