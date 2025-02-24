#include "minmax.h"

int negaMax(Board board, int depth, int alpha, int beta) {
    if (depth == 0) {
        return board.sideToMove == 0 ? evaluate(board) : -evaluate(board);
    }

    Move moves[256];
    int numMoves = generateMoves(board, moves);

    for (int i = 0; i < numMoves; i++) {
        Board newBoard = makeMove(board, moves[i]);
        int score = -negaMax(newBoard, depth - 1, -beta, -alpha);

        if (score >= beta) {
            return beta;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}