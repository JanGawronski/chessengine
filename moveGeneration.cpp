#include "Board.h"
#include "Move.h"
#define WHITE 0
#define BLACK 1


int generateMoves(Board board, Move moves[]) {
    int pawnMoves = generatePawnMoves(board, moves, 0);
    return pawnMoves;
}


int generatePawnMoves(Board board, Move moves[], int numMoves) {
    int playerIndex = 0 ? board.sideToMove == WHITE : 6;
    int opponentIndex = 6 ? board.sideToMove == WHITE : 0;
    int pawnMove = 8 ? board.sideToMove == WHITE : -8;
    uint64_t pawns = board.bitboards[playerIndex];

    uint64_t playerPiecesBitboard = 0;
    for (int i = playerIndex; i < playerIndex + 6; i++) {
        playerPiecesBitboard |= board.bitboards[i];
    }
    uint64_t opponentPiecesBitboard = 0;
    for (int i = opponentIndex; i < opponentIndex + 6; i++) {
        opponentPiecesBitboard |= board.bitboards[i];
    }
    uint64_t occupationBitboard = playerPiecesBitboard | opponentPiecesBitboard;

    // Center board pawn moves

    for (int i = 16; i < 48; i++) {
        if (pawns & (1ULL << i)) {
            if (!(occupationBitboard & (1ULL << (i + pawnMove)))) {
                moves[numMoves++] = {i, i + pawnMove, 0, 0, 0, 0, 0};
            }
        }
    }

    // Pawn double moves

    int firstRankStart = 8 ? board.sideToMove == WHITE : 48;
    int firstRankStartEnd = 15 ? board.sideToMove == WHITE : 55;
    
    for (int i = firstRankStart; i <= firstRankStartEnd; i++) {
        if (pawns & (1ULL << i)) {
            if (!(occupationBitboard & (1ULL << (i + pawnMove))) && !(occupationBitboard & (1ULL << (i + 2 * pawnMove)))) {
                moves[numMoves++] = {i, i + 2 * pawnMove, 0, 0, 0, 0, 0};
            }
        }
    }

    // Promotion moves

    int lastRankStart = 48 ? board.sideToMove == WHITE : 8;
    int lastRankEnd = 55 ? board.sideToMove == WHITE : 15;

    for (int i = lastRankStart; i <= lastRankEnd; i++) {
        if (pawns & (1ULL << i)) {
            if (!(occupationBitboard & (1ULL << (i + pawnMove)))) {
                moves[numMoves++] = {i, i + pawnMove, 0, 1, 0, 0, 0};
                moves[numMoves++] = {i, i + pawnMove, 0, 2, 0, 0, 0};
                moves[numMoves++] = {i, i + pawnMove, 0, 3, 0, 0, 0};
                moves[numMoves++] = {i, i + pawnMove, 0, 4, 0, 0, 0};
            }
        }
    }

    return numMoves;
}

int generateKnightMoves(Board board, Move moves[], int numMoves) {
    int playerIndex = 0 ? board.sideToMove == WHITE : 6;
    int opponentIndex = 6 ? board.sideToMove == WHITE : 0;
    uint64_t knights = board.bitboards[playerIndex + 1];

    uint64_t playerPieces = 0;
    for (int i = playerIndex; i < playerIndex + 6; i++) {
        playerPieces |= board.bitboards[i];
    }
    uint64_t opponentPieces = 0;
    for (int i = opponentIndex; i < opponentIndex + 6; i++) {
        opponentPieces |= board.bitboards[i];
    }

    for (int i = 0; i < 64; i++) {
        if (knights & (1ULL << i)) {
            uint64_t movesBitboard = 0;
            if (i % 8 < 6 && i / 8 < 7) {
                movesBitboard |= 1ULL << (i + 17);
            }
            if (i % 8 < 7 && i / 8 < 6) {
                movesBitboard |= 1ULL << (i + 10);
            }
            if (i % 8 < 7 && i / 8 > 0) {
                movesBitboard |= 1ULL << (i - 6);
            }
            if (i % 8 < 6 && i / 8 > 1) {
                movesBitboard |= 1ULL << (i - 15);
            }
            movesBitboard &= ~playerPieces;
            for (int j = 0; j < 64; j++) {
                if (movesBitboard & (1ULL << j)) {
                    if (opponentPieces & (1ULL << j)) {
                        for (int k = 0; k < 6; k++) {
                            if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                                moves[numMoves++] = {i, j, 1, 0, k + 1, 0, 0};
                            }
                        }
                    } else {
                        moves[numMoves++] = {i, j, 1, 0, 0, 0, 0};
                    }
                }
            }
        }
    }
}
