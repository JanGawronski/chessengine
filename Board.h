#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <string>

struct Board {
    uint64_t bitboards[12]; // 0-5: white pawns, knights, bishops, rooks, queens, king, 6-11: black pawns, knights, bishops, rooks, queens, king
    int sideToMove; // 0: white, 1: black
    int castlingRights; // 0-15: white king side, white queen side, black king side, black queen side
    int enPassantSquare; // 0: none or square index
    uint64_t zobristKey; // 64-bit hash key
};

inline std::string toString(Board board) {
    std::string boardString = board.sideToMove == 0 ? "White to move\n" : "Black to move\n";
    for (int i = 0; i < 64; i++) {
        bool found = false;
        for (int j = 0; j < 6; j++) {
            if (board.bitboards[j] & (1ULL << i)) {
                boardString += "PNBRQK"[j];
                found = true;
                break;
            }
        }
        if (!found) {
            for (int j = 6; j < 12; j++) {
                if (board.bitboards[j] & (1ULL << i)) {
                    boardString += "pnbrqk"[j - 6];
                    found = true;
                    break;
                }
            }
        }
        
        if (!found) {
            boardString += " ";
        }
        if (i % 8 == 7) {
            boardString += "\n";
        }
    }
    return boardString;
}

#endif // BOARD_H