#include "boardManipulation.h"

#define WHITE 0
#define BLACK 1

Board makeMove(Board board, Move move) {
    int playerIndex = board.sideToMove == WHITE ? 0 : 6;
    int opponentIndex = board.sideToMove == WHITE ? 6 : 0;
    Board newBoard = board;

    // Update bitboards
    newBoard.bitboards[move.piece + playerIndex] &= ~(1ULL << move.from);
    newBoard.bitboards[move.piece + playerIndex] |= 1ULL << move.to;

    if (move.capture) {
        newBoard.bitboards[move.capture - 1 + opponentIndex] &= ~(1ULL << move.to);
    }

    if (move.promotion) {
        newBoard.bitboards[move.promotion + playerIndex] |= 1ULL << move.to;
        newBoard.bitboards[move.piece + playerIndex] &= ~(1ULL << move.to);
    }

    // Update side to move
    newBoard.sideToMove = 1 - board.sideToMove;

    return newBoard;
}