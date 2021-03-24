#ifndef B_AIA_500_PAR_5_1_GOMOKU_VICTOR_HAMON_INCEPHALON_H
#define B_AIA_500_PAR_5_1_GOMOKU_VICTOR_HAMON_INCEPHALON_H

#include "BoardInstance.h"
#include <ctime>

class Incephalon : public BoardInstance {
private:
    int eval[8] = {0, 2, 12, 18, 96, 144, 800, 1200};

public:
    int total = 0;
    int hashCount = 0;
    int searchDepth = 0;
    int time_left = 10000000;
    int timeout_turn = 5000;
    int timeout_match = 10000000;
    int ThinkTime = 0;
    Point bestPoint;
    Node bestNode;
    clock_t start;
    bool stopIncephalon = false;

    Vector computeMainNode();
    Point browseNodes(int depth, int alpha, int beta, Node *pnode);
    void putPawn(Vector next);
    void recordMove(int depth, int value, int hashf, Vector best);
    Vector getNextPossibleMove(moveList &moveList);
    int removeNodes(Vector *move, Point *cand, int Csize);
    int getPossibleMoves(Vector *move);
    Vector getBestMove();
    int evaluateMove(Singleton *pSingleton);
    int computeHash(int depth, int alpha, int beta);
    int computeAlphaBetaPruning(int depth, int alpha, int beta, Node *pnode);
    int rateMove();
    static void sort(Point *a, int n);
    [[nodiscard]] int getTime() const;
    [[nodiscard]] int stopClock() const;
};

#endif //B_AIA_500_PAR_5_1_GOMOKU_VICTOR_HAMON_INCEPHALON_H
