#include "transpositionTable.h"

#define TABLE_SIZE (static_cast<uint64_t>(1) << 20)

uint64_t random64() {
    static std::mt19937_64 rng(std::random_device{}());
    static std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
    return dist(rng);
}

TTEntry transpositionTable[TABLE_SIZE];

void storeEntry(uint64_t key, int depth, int score, BoundType flag) {
    uint64_t index = key % TABLE_SIZE;
    if (transpositionTable[index].key == 0 || transpositionTable[index].depth <= depth) {
        transpositionTable[index].key = key;
        transpositionTable[index].depth = depth;
        transpositionTable[index].score = score;
        transpositionTable[index].flag = flag;
    }
}

TTEntry& probeEntry(uint64_t key) {
    if (transpositionTable[key % TABLE_SIZE].key == key) {
        return transpositionTable[key % TABLE_SIZE];
    }
    static TTEntry emptyEntry = {0, 0, 0, EXACT};
    return emptyEntry;
}

uint64_t positionKeys[12][64]; // 12 pieces, 64 squares
uint64_t enPassantKeys[8];     // 8 possible files for en passant
uint64_t castlingKeys[2];      // 0 for white, 1 for black
uint64_t sideKey;              // Key for side to move

void initKeys() {
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 64; j++) {
            positionKeys[i][j] = random64();
        }
    }
    for (int i = 0; i < 8; i++) {
        enPassantKeys[i] = random64();
    }
    castlingKeys[0] = random64();
    castlingKeys[1] = random64();
    sideKey = random64();
}

uint64_t calculateZobristKey(Board board) {
    uint64_t key = 0;
    for (int i = 0; i < 12; i++) {
        while (board.bitboards[i]) {
            int square = __builtin_ctzll(board.bitboards[i]);
            key ^= positionKeys[i][square];
            board.bitboards[i] &= board.bitboards[i] - 1;
        }
    }
    if (board.enPassantSquare != 0) {
        key ^= enPassantKeys[board.enPassantSquare - 1];
    }
    //key ^= castlingKeys[board.castlingRights];
    if (board.sideToMove == 1) {
        key ^= sideKey;
    }
    return key;
}

bool seenPositions[TABLE_SIZE];

void seePosition(uint64_t key) {
    seenPositions[key % TABLE_SIZE] = true;
}

void unseePosition(uint64_t key) {
    seenPositions[key % TABLE_SIZE] = false;
}

bool hasSeenPosition(uint64_t key) {
    return seenPositions[key % TABLE_SIZE];
}