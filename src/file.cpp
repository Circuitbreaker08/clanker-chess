#include <iostream>
#include <string>
#include <fstream>
#include <mutex>
#include <memory>

#include <crow.h>
#include "file.hpp"

void write_file(std::string path, std::string content) {
    std::ofstream file(path);
    file << content;
    file.close();
}

std::string read_file(std::string path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;

        throw std::runtime_error("Failed to open file: " + path);
    }

    std::string output;
    std::string line;
    while (getline(file, line)) {
        output += line + "\n";
    }

    file.close();

    return output;
}

crow::json::rvalue load_json(std::string path) {
    return crow::json::load(read_file(path));
}

Tracker::Tracker(std::string path) {
    this->path = path;
    if (!std::filesystem::exists(path)) {
        write_file(path, "{}");
    } else {
        json = load_json(path);
    }
}

std::shared_ptr<Tracker> Tracker::get_ptr(std::string path) {
    return std::make_shared<Tracker>(Tracker(path));
}

void Tracker::save() {
    write_file(path, json.dump());
}