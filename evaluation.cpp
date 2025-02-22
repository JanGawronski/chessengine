#include "Board.h"

int evaluate(Board board) {
    int whitePawns = __builtin_popcountll(board.bitboards[0]);
    int whiteKnights = __builtin_popcountll(board.bitboards[1]);
    int whiteBishops = __builtin_popcountll(board.bitboards[2]);
    int whiteRooks = __builtin_popcountll(board.bitboards[3]);
    int whiteQueens = __builtin_popcountll(board.bitboards[4]);
    int whiteKing = __builtin_popcountll(board.bitboards[5]);

    int blackPawns = __builtin_popcountll(board.bitboards[6]);
    int blackKnights = __builtin_popcountll(board.bitboards[7]);
    int blackBishops = __builtin_popcountll(board.bitboards[8]);
    int blackRooks = __builtin_popcountll(board.bitboards[9]);
    int blackQueens = __builtin_popcountll(board.bitboards[10]);
    int blackKing = __builtin_popcountll(board.bitboards[11]);

    return 100 * (whitePawns - blackPawns) +
           300 * (whiteKnights - blackKnights) +
           300 * (whiteBishops - blackBishops) +
           500 * (whiteRooks - blackRooks) +
           900 * (whiteQueens - blackQueens) +
           200000 * (whiteKing - blackKing);
}