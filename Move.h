#ifndef MOVE_H
#define MOVE_H

struct Move {
    int from; // 0-63
    int to; // 0-63
    int piece; // 0: pawn, 1: knight, 2: bishop, 3: rook, 4: queen, 5: king
    int promotion; // 0: none, 1: knight, 2: bishop, 3: rook, 4: queen
    int capture; // 0: none, 1: pawn, 2: knight, 3: bishop, 4: rook, 5: queen
    int enPassant; // 0: none, 1: white, 2: black
    int castling; // 0: none, 1: white king side, 2: white queen side, 3: black king side, 4: black queen side
};

#endif // MOVE_H