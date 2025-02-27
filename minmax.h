#ifndef MINMAX_H
#define MINMAX_H

#include <cstdint>
#include "Board.h"
#include "Move.h"
#include "evaluation.h"
#include "boardManipulation.h"
#include "moveGeneration.h"
#include "transpositionTable.h"

int negaMax(Board board, int depth, int alpha, int beta);

#endif // "MINMAX_H"