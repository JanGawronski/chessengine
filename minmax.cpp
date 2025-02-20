#include <cstdint>

struct Board {
    uint64_t bitboards[12]; // 0-5: white pawns, knights, bishops, rooks, queens, king
                            // 6-11: black pawns, knights, bishops, rooks, queens, king
    int sideToMove; // 0: white, 1: black
    int castlingRights; // 0-15: white king side, white queen side, black king side, black queen side
    int enPassantSquare; // 0: none or square index
    uint64_t zobristKey; // 64-bit hash key
};

struct Move {
    int from; // 0-63
    int to; // 0-63
    int promotion; // 0: none, 1: knight, 2: bishop, 3: rook, 4: queen
    int capture; // 0: none, 1: pawn, 2: knight, 3: bishop, 4: rook, 5: queen
    int enPassant; // 0: none, 1: white, 2: black
    int castling; // 0: none, 1: white king side, 2: white queen side, 3: black king side, 4: black queen side
};



int negaMax(Board board, int depth, int alpha, int beta) {
    if (depth == 0) {
        return evaluate(board);
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