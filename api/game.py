import json

import counter

class Game:
    def __init__(self):
        with open("default_board.json") as f: self.position = json.loads(f.read())
        self.white_to_move = True
        self.id = counter.get_parameter("game")

    def get_state(self):
        return self.position
    
    def piece_at(self, position: tuple[int, int]):
        for piece in self.position:
            if piece["x"] == position[0] and piece["y"] == position[1]:
                return piece
        return None
    
    def move(self, origin: tuple[int, int], target: tuple[int, int], is_white: bool, promotion: None | str = None) -> bool:
        piece = self.piece_at(origin)
        if origin == target or piece == None or piece["is_white"] != is_white or self.piece_at(target)["is_white"] == is_white:
            return False
        
        if piece == "pawn":
            pass
        elif piece == "rook":
            if origin[0] != target[0] and origin[1] != target[1]:
                return False
            move_axis: int = 1 if origin[0] == target[0] else 0
            i = min(origin[move_axis], target[move_axis]) + 1
            while i < max(origin[move_axis], target[move_axis]):
                if self.piece_at(
                    (
                        origin[0] if move_axis != 0 else i,
                        origin[1] if move_axis != 1 else i
                    )
                ) != None:
                    return False
                i += 1
        elif piece == "knight":
            pass
        elif piece == "bishop":
            pass
        elif piece == "queen":
            pass
        elif piece == "king":
            pass

if __name__ == "__main__":
    # "\033[30m" black
    # "\033[37m" white
    # "\033[0m" clear

    game = Game()

    def print_board():
        for rank in reversed(range(1, 9)):
            for file in range(1, 9):
                for piece in game.position["state"]:
                    if piece["file"] == file and piece["rank"] == rank:
                        print(
                            ("\033[37m" if piece["is_white"] else "\033[30m"),
                            {
                                "pawn": "-",
                                "rook": "R",
                                "knight": "N",
                                "bishop": "B",
                                "king": "K",
                                "queen": "Q"
                            }[piece["piece"]],
                            end=" "
                        )
                        break
                else:
                    print("\033[0m * ", end="")
            print()

    print_board()

    move = input("White move: ")