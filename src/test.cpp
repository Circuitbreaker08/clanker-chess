#include <iostream>
#include <fstream>

#include "json/json.h"

int main() {
    Json::Value root;
    std::ifstream config_doc("../data/default_board.json", std::ifstream::binary);
    config_doc >> root;
    std::cout << root << std::endl;
    return 0;
}