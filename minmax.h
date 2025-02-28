#ifndef MINMAX_H
#define MINMAX_H

#include <cstdint>
#include <time.h>
#include "Board.h"
#include "Move.h"
#include "evaluation.h"
#include "boardManipulation.h"
#include "moveGeneration.h"
#include "transpositionTable.h"

int negaMax(Board board, int depth, int alpha, int beta);

int getEvaluation(Board board, time_t timeToSearch);

Move getBestMove(Board board);

#endif // "MINMAX_H"