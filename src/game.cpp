#include <memory>
#include <utility>

struct Position {
    bool castle_king;
    bool castle_queen;
    std::unique_ptr<std::pair<int, int>> en_passant;

    
};

class Game {
    Game() {

    }
};