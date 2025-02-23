#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

#include "Board.h"
#include "Move.h"

int generateMoves(Board board, Move moves[]);
int generatePawnMoves(Board board, Move moves[], int numMoves);
int generateKnightMoves(Board board, Move moves[], int numMoves);
int generateBishopMoves(Board board, Move moves[], int numMoves);
int generateRookMoves(Board board, Move moves[], int numMoves);
int generateQueenMoves(Board board, Move moves[], int numMoves);
int generateKingMoves(Board board, Move moves[], int numMoves);

#endif // MOVEGENERATION_H