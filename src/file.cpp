#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <shared_mutex>
#include <memory>
#include <functional>

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



Tracker::Tracker(std::string path, std::function<void(crow::json::wvalue& json)> default_file_contents_init) {
    this->path = path;
    std::thread(
        [](Tracker* tracker, std::function<void(crow::json::wvalue& json)> default_file_contents_init){
            tracker->mutex->lock();
            while (!Tracker::initialized);
            if (!std::filesystem::exists(tracker->path)) {
                tracker->json = crow::json::wvalue::object();
                default_file_contents_init(tracker->json);
                tracker->save();
            } else {
                tracker->json = load_json(tracker->path);
            }
            tracker->mutex->unlock();
        },
        this,
        default_file_contents_init
    ).detach();
}

Tracker::Tracker(std::string path) : Tracker::Tracker(path, [](crow::json::wvalue& json){}) {}; 

void Tracker::init() {
    initialized = true;
}

void Tracker::save() {
    write_file(path, json.dump());
}

bool Tracker::initialized = false;