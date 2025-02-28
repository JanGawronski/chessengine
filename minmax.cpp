#include "minmax.h"

#define CHECKMATE 10000

int negaMax(Board board, int depth, int alpha, int beta) {
    if (hasSeenPosition(board.zobristKey)) {
        return 0;
    }
    
    TTEntry ttEntry = probeEntry(board.zobristKey);
    if (ttEntry.key != 0 && ttEntry.depth >= depth) {
        if (ttEntry.flag == EXACT) return ttEntry.score;
        if (ttEntry.flag == LOWERBOUND && ttEntry.score > alpha) alpha = ttEntry.score;
        if (ttEntry.flag == UPPERBOUND && ttEntry.score < beta) beta = ttEntry.score;
        if (alpha >= beta)
            return ttEntry.score;
    }

    if (depth == 0) {
        return board.sideToMove == 0 ? evaluate(board) : -evaluate(board);
    }

    seePosition(board.zobristKey);

    int bestScore = -1000000;
    Move moves[256];
    int numMoves = generateMoves(board, moves);

    for (int i = 0; i < numMoves; i++) {
        Board newBoard = makeMove(board, moves[i]);

        int score = -negaMax(newBoard, depth - 1, -beta, -alpha);

        if (score > bestScore) {
            bestScore = score;
        }

        if (score >= beta) {
            break;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    BoundType flag;
    if (bestScore <= alpha) {
        flag = UPPERBOUND;
    } else if (bestScore >= beta) {
        flag = LOWERBOUND;
    } else {
        flag = EXACT;
    }
    storeEntry(board.zobristKey, depth, bestScore, flag);
    
    unseePosition(board.zobristKey);

    return bestScore;

}

int getEvaluation(Board board, time_t timeToSearch) {
    time_t endTime = time(NULL) + timeToSearch;
    int depth = 0;
    int score = 0;
    while (time(NULL) < endTime) {
        score = negaMax(board, depth, -1000000, 1000000);
        if (score >= CHECKMATE || score <= -CHECKMATE) {
            break;
        }
        depth++;
    }

    return score;
}

Move getBestMove(Board board) {
    Move bestMove;
    
    int bestScore = -1000000;
    Move moves[256];
    int numMoves = generateMoves(board, moves);
    for (int i = 0; i < numMoves; i++) {
        Board newBoard = makeMove(board, moves[i]);

        int score = probeEntry(newBoard.zobristKey).score;

        if (score > bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }

    return bestMove;
}