#include <optional>
#include <utility>

struct Position {
    bool white_castle_king;
    bool white_castle_queen;
    bool black_castle_king;
    bool black_castle_queen;
    std::optional<std::pair<int, int>> en_passant;
};

class Game {
    Game() {

    }
};