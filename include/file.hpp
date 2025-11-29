#include <string>

#include <crow.h>

void write_file(std::string path, std::string content);

std::string read_file(std::string path);

crow::json::rvalue load_json(std::string path);