#ifndef PBRAIN_INCEPHALON_STRUCTURES_H
#define PBRAIN_INCEPHALON_STRUCTURES_H
#include "Settings.h"

struct Vector {
    int x;
    int y;
};


struct Point {
    Vector p;
    int value;
};


struct Singleton {
    int piece;
    int IsCand;
    int pattern[2][4];
};


struct Hashe {
    uint64_t key;
    int depth;
    int hashf;
    int value;
};

struct Pv {
    uint64_t key;
    Vector best;
};


struct Node {
    int n;
    Vector moves[MaxDepth];
};

struct moveList {
    int phase, n, index;
    bool first;
    Vector hashMove;
    Vector moves[64];
};

#endif //PBRAIN_INCEPHALON_STRUCTURES_H
