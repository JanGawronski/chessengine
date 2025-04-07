#include "minmax.h"

#define CHECKMATE 10000


bool chooseMove(Board boards[256], int numMoves, Board &bestBoard, bool searchInTT);

int negaMax(Board board, int depth, int alpha, int beta) {
    if (hasSeenPosition(board.zobristKey)) {
        return 0;
    }
    
    TTEntry ttEntry = probeEntry(board.zobristKey);

    if (ttEntry.key != 0 && (ttEntry.score == CHECKMATE || ttEntry.score == -CHECKMATE)) {
        return ttEntry.score;
    }

    if (ttEntry.key != 0 && ttEntry.depth >= depth) {
        if (ttEntry.flag == EXACT) return ttEntry.score;
        if (ttEntry.flag == LOWERBOUND && ttEntry.score > alpha) alpha = ttEntry.score;
        if (ttEntry.flag == UPPERBOUND && ttEntry.score < beta) beta = ttEntry.score;
        if (alpha >= beta)
            return ttEntry.score;
    }

    if (depth == 0) {
        int score = board.sideToMove == 0 ? evaluate(board) : -evaluate(board);
        storeEntry(board.zobristKey, depth, score, EXACT);
        return score;
    }

    seePosition(board.zobristKey);

    int bestScore = -1000000;
    Move moves[256];
    int numMoves = generateMoves(board, moves);

    Board newBoards[256];
    for (int i = 0; i < numMoves; i++) {
        newBoards[i] = makeMove(board, moves[i]);
    }

    bool searchInTT = true;
    for (int i = numMoves - 1; i >= 0; i--) {
        Board newBoard; 
        searchInTT = chooseMove(newBoards, i, newBoard, searchInTT);

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

bool chooseMove(Board boards[256], int numMoves, Board &bestBoard, bool searchInTT) {
    bestBoard = boards[numMoves];
    if (!searchInTT)
        return false;

    int previousBestScore = -1000000;
    int bestMoveIndex = numMoves;
    
    for (int i = numMoves; i >= 0; i--) {
        TTEntry ttEntry = probeEntry(boards[i].zobristKey);
        if (ttEntry.key == 0) {
            continue;
        }
        
        int score = -ttEntry.score;
        if (score > previousBestScore) {
            previousBestScore = score;
            bestBoard = boards[i];
            bestMoveIndex = i;
        }
    }

    if (previousBestScore == -1000000)
        return false;
    
    boards[bestMoveIndex] = boards[numMoves];
    return true;
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

        int score = -probeEntry(newBoard.zobristKey).score;

        if (score > bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }

    return bestMove;
}