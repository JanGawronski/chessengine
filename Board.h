#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

struct Board {
    uint64_t bitboards[12]; // 0-5: white pawns, knights, bishops, rooks, queens, king
                            // 6-11: black pawns, knights, bishops, rooks, queens, king
    int sideToMove; // 0: white, 1: black
    int castlingRights; // 0-15: white king side, white queen side, black king side, black queen side
    int enPassantSquare; // 0: none or square index
    uint64_t zobristKey; // 64-bit hash key
};

#endif // BOARD_H