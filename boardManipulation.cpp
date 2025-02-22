#include "Board.h"
#include "Move.h"

Board makeMove(Board board, Move move) {
    Board newBoard = board;

    // Update bitboards
    newBoard.bitboards[move.piece] &= ~(1ULL << move.from);
    newBoard.bitboards[move.piece] |= 1ULL << move.to;

    if (move.capture) {
        int capturePiece = move.capture + 5 * (1 - board.sideToMove);
        newBoard.bitboards[capturePiece] &= ~(1ULL << move.to);
    }

    // Update side to move
    newBoard.sideToMove = 1 - board.sideToMove;

    return newBoard;
}