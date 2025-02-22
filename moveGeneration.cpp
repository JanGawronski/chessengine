#include "Board.h"
#include "Move.h"
#define WHITE 0
#define BLACK 1


int generateMoves(Board board, Move moves[]) {
    return generatePawnMoves(board, moves);
}


int generatePawnMoves(Board board, Move moves[]) {
    uint64_t whitePieces = 0;
    for (int i = 0; i < 6; i++) {
        whitePieces |= board.bitboards[i];
    }

    uint64_t blackPieces = 0;
    for (int i = 6; i < 12; i++) {
        blackPieces |= board.bitboards[i];
    }

    uint64_t occupationBitboard = whitePieces | blackPieces;

    uint64_t playerPiecesBitboard = whitePieces ? board.sideToMove == WHITE : blackPieces;
    uint64_t opponentPiecesBitboard = blackPieces ? board.sideToMove == WHITE : whitePieces;

    int playerIndex = 0 ? board.sideToMove == WHITE : 6;
    int opponentIndex = 6 ? board.sideToMove == WHITE : 0;

    int numMoves = 0;


    // Center board pawn moves

    int pawnMove = 8 ? board.sideToMove == WHITE : -8;
    uint64_t pawns = board.bitboards[playerIndex];
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
