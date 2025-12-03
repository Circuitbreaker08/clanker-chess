#ifndef CLANKER_FILE
#define CLANKER_FILE

#include <mutex>
#include <string>
#include <memory>
#include <functional>

#include <crow.h>

void write_file(std::string path, std::string content);

std::string read_file(std::string path);

crow::json::rvalue load_json(std::string path);

struct Tracker {
    Tracker(std::string path);

    Tracker(std::string path, std::function<void(crow::json::wvalue& json)> default_file_contents_init);

    static void init();

    void save();

    std::string path;
    crow::json::wvalue json;
    std::shared_ptr<std::mutex> mutex;

    protected:
    static bool initialized;
};

#endif