#include "moveGeneration.h"
#define WHITE 0
#define BLACK 1


int generateMoves(Board board, Move moves[]) {
    int pawnMoves = generatePawnMoves(board, moves, 0);
    int knightMoves = generateKnightMoves(board, moves, pawnMoves);
    int bishopMoves = generateBishopMoves(board, moves, knightMoves);
    int rookMoves = generateRookMoves(board, moves, bishopMoves);
    int queenMoves = generateQueenMoves(board, moves, rookMoves);
    int kingMoves = generateKingMoves(board, moves, queenMoves);
    return kingMoves;
}


int generatePawnMoves(Board board, Move moves[], int numMoves) {
    int playerIndex = board.sideToMove == WHITE ? 0 : 6;
    int opponentIndex = board.sideToMove == WHITE ? 6 : 0;
    int pawnMove = board.sideToMove == WHITE ? 8 : -8;
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

    int secondRankStart = board.sideToMove == WHITE ? 8 : 48;
    int secondRankEnd = board.sideToMove == WHITE ? 15 : 55;
    
    for (int i = secondRankStart; i <= secondRankEnd; i++) {
        if (pawns & (1ULL << i)) {
            if (!(occupationBitboard & (1ULL << (i + pawnMove))) && !(occupationBitboard & (1ULL << (i + 2 * pawnMove)))) {
                moves[numMoves++] = {i, i + 2 * pawnMove, 0, 0, 0, 0, 0};
            }
        }
    }

    // Promotion moves

    int secondToLastRankStart = board.sideToMove == WHITE ? 48 : 8;
    int secondToLastRankEnd = board.sideToMove == WHITE ? 55 : 15;

    for (int i = secondToLastRankStart; i <= secondToLastRankEnd; i++) {
        if (pawns & (1ULL << i)) {
            if (!(occupationBitboard & (1ULL << (i + pawnMove)))) {
                moves[numMoves++] = {i, i + pawnMove, 0, 1, 0, 0, 0};
                moves[numMoves++] = {i, i + pawnMove, 0, 2, 0, 0, 0};
                moves[numMoves++] = {i, i + pawnMove, 0, 3, 0, 0, 0};
                moves[numMoves++] = {i, i + pawnMove, 0, 4, 0, 0, 0};
            }
        }
    }

    // Capture moves

    int leftCaptureMove = board.sideToMove == WHITE ? 9 : -7;
    int rightCaptureMove = board.sideToMove == WHITE ? 7 : -9;

    uint64_t leftCaptureBitboard = ((pawns & ~0x8080808080808080ULL) << leftCaptureMove) & opponentPiecesBitboard;
    uint64_t rightCaptureBitboard = ((pawns & ~0x0101010101010101ULL) << rightCaptureMove) & opponentPiecesBitboard;

    for (int i = 8; i < 56; i++) {
        if (leftCaptureBitboard & (1ULL << i)) {
            for (int j = 0; j < 6; j++) {
                if (board.bitboards[opponentIndex + j] & (1ULL << (i + leftCaptureMove))) {
                    moves[numMoves++] = {i - leftCaptureMove, i, 0, 0, j + 1, 0, 0};
                }
            }
        }
        if (rightCaptureBitboard & (1ULL << i)) {
            for (int j = 0; j < 6; j++) {
                if (board.bitboards[opponentIndex + j] & (1ULL << (i + rightCaptureMove))) {
                    moves[numMoves++] = {i - rightCaptureMove, i, 0, 0, j + 1, 0, 0};
                }
            }
        }
    }

    for (int i = secondRankStart - pawnMove; i <= secondRankEnd - pawnMove; i++) {
        if (leftCaptureBitboard & (1ULL << i)) {
            for (int j = 0; j < 6; j++) {
                if (board.bitboards[opponentIndex + j] & (1ULL << (i + leftCaptureMove))) {
                    moves[numMoves++] = {i - leftCaptureMove, i, 0, 1, j + 1, 0, 0};
                    moves[numMoves++] = {i - leftCaptureMove, i, 0, 2, j + 1, 0, 0};
                    moves[numMoves++] = {i - leftCaptureMove, i, 0, 3, j + 1, 0, 0};
                    moves[numMoves++] = {i - leftCaptureMove, i, 0, 4, j + 1, 0, 0};
                }
            }
        }
        if (rightCaptureBitboard & (1ULL << i)) {
            for (int j = 0; j < 6; j++) {
                if (board.bitboards[opponentIndex + j] & (1ULL << (i + rightCaptureMove))) {
                    moves[numMoves++] = {i - rightCaptureMove, i, 0, 1, j + 1, 0, 0};
                    moves[numMoves++] = {i - rightCaptureMove, i, 0, 2, j + 1, 0, 0};
                    moves[numMoves++] = {i - rightCaptureMove, i, 0, 3, j + 1, 0, 0};
                    moves[numMoves++] = {i - rightCaptureMove, i, 0, 4, j + 1, 0, 0};
                }
            }
        }
    }

    return numMoves;
}

int generateKnightMoves(Board board, Move moves[], int numMoves) {
    int playerIndex = board.sideToMove == WHITE ? 0 : 6;
    int opponentIndex = board.sideToMove == WHITE ? 6 : 0;
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
    return numMoves;
}

int generateBishopMoves(Board board, Move moves[], int numMoves) {
    int playerIndex = board.sideToMove == WHITE ? 0 : 6;
    int opponentIndex = board.sideToMove == WHITE ? 6 : 0;
    uint64_t bishops = board.bitboards[playerIndex + 2];

    uint64_t playerPieces = 0;
    for (int i = playerIndex; i < playerIndex + 6; i++) {
        playerPieces |= board.bitboards[i];
    }
    uint64_t opponentPieces = 0;
    for (int i = opponentIndex; i < opponentIndex + 6; i++) {
        opponentPieces |= board.bitboards[i];
    }

    for (int i = 0; i < 64; i++) {
        if (bishops & (1ULL << i)) {
            for (int j = i + 9; j % 8 != 0 && j / 8 < 8; j += 9) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 2, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 2, 0, 0, 0, 0};
            }
            for (int j = i + 7; j % 8 != 7 && j / 8 < 8; j += 7) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 2, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 2, 0, 0, 0, 0};
            }
            for (int j = i - 9; j % 8 != 7 && j / 8 >= 0; j -= 9) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 2, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 2, 0, 0, 0, 0};
            }
            for (int j = i - 7; j % 8 != 0 && j / 8 >= 0; j -= 7) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 2, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 2, 0, 0, 0, 0};
            }
        }
    }
    return numMoves;
}

int generateRookMoves(Board board, Move moves[], int numMoves) {
    int playerIndex = board.sideToMove == WHITE ? 0 : 6;
    int opponentIndex = board.sideToMove == WHITE ? 6 : 0;
    uint64_t rooks = board.bitboards[playerIndex + 3];

    uint64_t playerPieces = 0;
    for (int i = playerIndex; i < playerIndex + 6; i++) {
        playerPieces |= board.bitboards[i];
    }
    uint64_t opponentPieces = 0;
    for (int i = opponentIndex; i < opponentIndex + 6; i++) {
        opponentPieces |= board.bitboards[i];
    }

    for (int i = 0; i < 64; i++) {
        if (rooks & (1ULL << i)) {
            for (int j = i + 8; j / 8 < 8; j += 8) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 3, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 3, 0, 0, 0, 0};
            }
            for (int j = i - 8; j / 8 >= 0; j -= 8) {
                if (playerPieces & (1ULL << j) || j < 0) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 3, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 3, 0, 0, 0, 0};
            }
            for (int j = i + 1; j % 8 != 0; j++) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 3, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 3, 0, 0, 0, 0};
            }
            for (int j = i - 1; j % 8 != 7; j--) {
                if (playerPieces & (1ULL << j) || j < 0) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 3, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 3, 0, 0, 0, 0};
            }
        }
    }
    return numMoves;
}

int generateQueenMoves(Board board, Move moves[], int numMoves) {
    int playerIndex = board.sideToMove == WHITE ? 0 : 6;
    int opponentIndex = board.sideToMove == WHITE ? 6 : 0;
    uint64_t queens = board.bitboards[playerIndex + 4];

    uint64_t playerPieces = 0;
    for (int i = playerIndex; i < playerIndex + 6; i++) {
        playerPieces |= board.bitboards[i];
    }
    uint64_t opponentPieces = 0;
    for (int i = opponentIndex; i < opponentIndex + 6; i++) {
        opponentPieces |= board.bitboards[i];
    }

    for (int i = 0; i < 64; i++) {
        if (queens & (1ULL << i)) {
            for (int j = i + 9; j % 8 != 0 && j / 8 < 8; j += 9) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 4, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 4, 0, 0, 0, 0};
            }
            for (int j = i + 7; j % 8 != 7 && j / 8 < 8; j += 7) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 4, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 4, 0, 0, 0, 0};
            }
            for (int j = i - 9; j % 8 != 7 && j / 8 >= 0; j -= 9) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 4, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 4, 0, 0, 0, 0};
            }
            for (int j = i - 7; j % 8 != 0 && j / 8 >= 0; j -= 7) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 4, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 4, 0, 0, 0, 0};
            }
            for (int j = i + 8; j / 8 < 8; j += 8) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 4, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 4, 0, 0, 0, 0};
            }
            for (int j = i - 8; j / 8 >= 0; j -= 8) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 4, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 4, 0, 0, 0, 0};
            }
            for (int j = i + 1; j % 8 != 0; j++) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 4, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 4, 0, 0, 0, 0};
            }
            for (int j = i - 1; j % 8 != 7; j--) {
                if (playerPieces & (1ULL << j)) {
                    break;
                }
                if (opponentPieces & (1ULL << j)) {
                    for (int k = 0; k < 6; k++) {
                        if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                            moves[numMoves++] = {i, j, 4, 0, k + 1, 0, 0};
                        }
                    }
                    break;
                }
                moves[numMoves++] = {i, j, 4, 0, 0, 0, 0};
            }
        }
    }
    return numMoves;
}

int generateKingMoves(Board board, Move moves[], int numMoves) {
    int playerIndex = board.sideToMove == WHITE ? 0 : 6;
    int opponentIndex = board.sideToMove == WHITE ? 6 : 0;
    uint64_t king = board.bitboards[playerIndex + 5];

    uint64_t playerPieces = 0;
    for (int i = playerIndex; i < playerIndex + 6; i++) {
        playerPieces |= board.bitboards[i];
    }
    uint64_t opponentPieces = 0;
    for (int i = opponentIndex; i < opponentIndex + 6; i++) {
        opponentPieces |= board.bitboards[i];
    }

    for (int i = 0; i < 64; i++) {
        if (king & (1ULL << i)) {
            uint64_t movesBitboard = 0;
            if (i % 8 < 7) {
                movesBitboard |= 1ULL << (i + 1);
            }
            if (i % 8 > 0) {
                movesBitboard |= 1ULL << (i - 1);
            }
            if (i / 8 < 7) {
                movesBitboard |= 1ULL << (i + 8);
            }
            if (i / 8 > 0) {
                movesBitboard |= 1ULL << (i - 8);
            }
            if (i % 8 < 7 && i / 8 < 7) {
                movesBitboard |= 1ULL << (i + 9);
            }
            if (i % 8 > 0 && i / 8 < 7) {
                movesBitboard |= 1ULL << (i + 7);
            }
            if (i % 8 < 7 && i / 8 > 0) {
                movesBitboard |= 1ULL << (i - 7);
            }
            if (i % 8 > 0 && i / 8 > 0) {
                movesBitboard |= 1ULL << (i - 9);
            }
            movesBitboard &= ~playerPieces;
            for (int j = 0; j < 64; j++) {
                if (movesBitboard & (1ULL << j)) {
                    if (opponentPieces & (1ULL << j)) {
                        for (int k = 0; k < 6; k++) {
                            if (board.bitboards[opponentIndex + k] & (1ULL << j)) {
                                moves[numMoves++] = {i, j, 5, 0, k + 1, 0, 0};
                            }
                        }
                    } else {
                        moves[numMoves++] = {i, j, 5, 0, 0, 0, 0};
                    }
                }
            }
        }
    }
    return numMoves;
}