#include <string>
#include <memory>

#include <crow.h>

void write_file(std::string path, std::string content);

std::string read_file(std::string path);

crow::json::rvalue load_json(std::string path);

struct Tracker {
    // TODO - allow a callback for init if needed
    static std::shared_ptr<Tracker> get_ptr(std::string path);

    void save();

    std::string path;
    crow::json::wvalue json;
    std::mutex mutex;

    private:
    Tracker(std::string path);
};