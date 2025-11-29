#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

#define CROW_STATIC_DIRECTORY "web/"
#define CROW_STATIC_ENDPOINT "/<path>"

#include "crow.h"

#include "file.hpp"

crow::json::rvalue default_board;

typedef std::shared_ptr<Tracker> tracker;

tracker counter;
tracker token_registry;

int main() {
    // Set cwd to file location
    std::filesystem::current_path(std::filesystem::canonical("/proc/self/exe").parent_path());

    default_board = load_json("data/default_board.json");

    counter = Tracker::get_ptr("data/counter.json");
    token_registry = Tracker::get_ptr("data/token_registry.json");

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