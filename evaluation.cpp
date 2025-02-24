#include "evaluation.h"
#include "moveGeneration.h"

int evaluate(Board board) {
    if (board.bitboards[11] == 0) {
        return 10000;
    }
    if (board.bitboards[5] == 0) {
        return -10000;
    }

    int whitePawns = __builtin_popcountll(board.bitboards[0]);
    int whiteKnights = __builtin_popcountll(board.bitboards[1]);
    int whiteBishops = __builtin_popcountll(board.bitboards[2]);
    int whiteRooks = __builtin_popcountll(board.bitboards[3]);
    int whiteQueens = __builtin_popcountll(board.bitboards[4]);

    int blackPawns = __builtin_popcountll(board.bitboards[6]);
    int blackKnights = __builtin_popcountll(board.bitboards[7]);
    int blackBishops = __builtin_popcountll(board.bitboards[8]);
    int blackRooks = __builtin_popcountll(board.bitboards[9]);
    int blackQueens = __builtin_popcountll(board.bitboards[10]);

    Move moves[256];
    board.sideToMove = 0;
    int numMoves = generateMoves(board, moves);
    
    uint64_t whiteMobilityBitboard = 0;
    for (int i = 0; i < numMoves; i++) {
        whiteMobilityBitboard |= 1ULL << moves[i].to;

    }

    int whiteMobility = __builtin_popcountll(whiteMobilityBitboard);

    board.sideToMove = 1;
    numMoves = generateMoves(board, moves);

    uint64_t blackMobilityBitboard = 0;
    for (int i = 0; i < numMoves; i++) {
        blackMobilityBitboard |= 1ULL << moves[i].to;
    }

    int blackMobility = __builtin_popcountll(blackMobilityBitboard);

    return 10 * (whitePawns - blackPawns) +
           30 * (whiteKnights - blackKnights) +
           30 * (whiteBishops - blackBishops) +
           50 * (whiteRooks - blackRooks) +
           90 * (whiteQueens - blackQueens) +
                (whiteMobility - blackMobility);
}