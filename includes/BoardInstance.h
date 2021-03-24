#ifndef B_AIA_500_PAR_5_1_GOMOKU_VICTOR_HAMON_BOARDINSTANCE_H
#define B_AIA_500_PAR_5_1_GOMOKU_VICTOR_HAMON_BOARDINSTANCE_H

#include <cstdint>
#include "Structures.h"
#include "Settings.h"

class BoardInstance {
public:
    int step = 0;
    int size = 15;
    int b_start, b_end;
    uint64_t zobristKey = 0;
    uint64_t zobrist[2][MaxSize + 4][MaxSize + 4];
    Hashe hashTable[hashSize];
    Pv pvsTable[pvsSize];
    int typeTable[10][6][6][3];
    int patternTable[65536][2];
    int pval[8][8][8][8];
    Singleton pSingleton[MaxSize + 8][MaxSize + 8];
    Vector remMove[MaxSize * MaxSize];
    Point cand[256];
    bool IsLose[MaxSize + 4][MaxSize + 4];
    int myAI = Black;
    int opp = White;
    Point nodes[64];
    int nodeCount;
    int ply = 0;

    BoardInstance();
    ~BoardInstance();
    void removeLastStep();
    void restart();
    void updatePawn(int x, int y);
    int getHeuristicAssist(int len, int len2, int count, int block);
    void countPawns(Singleton *c, int role, int *type);
    bool checkPawnType(Singleton *c, int role, int type);
    void initZobrist();
    void initPawnType();
    void setSize(int _size);
    void playPawn(Vector next);
    int getPval(int a, int b, int c, int d);
    int getNodeSpec(int role, int key);
    int checkFlex3(int *node);
    int checkFlex4(const int *node);
    bool checkCoordinates(int x, int y);
    Singleton *getLastStep();
    int computeShortNode(const int *node);
    bool checkWin();
    uint64_t rand64();
    int getKey(int x, int y, int i);
};

#endif //B_AIA_500_PAR_5_1_GOMOKU_VICTOR_HAMON_BOARDINSTANCE_H
