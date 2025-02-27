#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include "Board.h"
#include <cstdint>
#include <cstdlib>
#include <random>

enum BoundType { EXACT, LOWERBOUND, UPPERBOUND };

struct TTEntry {
    uint64_t key;    // Zobrist hash for the position
    int depth;       // Depth of search when this entry was stored
    int score;       // Evaluation score
    BoundType flag;  // Type of bound: EXACT, LOWERBOUND, or UPPERBOUND
};

extern TTEntry transpositionTable[];

void storeEntry(uint64_t key, int depth, int score, BoundType flag);
TTEntry& probeEntry(uint64_t key);

extern uint64_t positionKeys[12][64]; // 12 pieces, 64 squares
extern uint64_t enPassantKeys[8];     // 8 possible files for en passant
extern uint64_t castlingKeys[2];      // 0 for white, 1 for black
extern uint64_t sideKey;              // Key for side to move

void initKeys();
uint64_t calculateZobristKey(Board board);

extern bool seenPositions[];

void seePosition(uint64_t key);
void unseePosition(uint64_t key);
bool hasSeenPosition(uint64_t key);

#endif // TRANSPOSITIONTABLE_H