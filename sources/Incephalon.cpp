#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <chrono>
#include <string>
#include <SharedFunctions.h>
#include "Incephalon.h"

int Incephalon::getTime() const {
    return (double) (clock() - start) / CLOCKS_PER_SEC * 1000;
}

int Incephalon::stopClock() const {
    if (timeout_turn >= time_left / 7) {
        return time_left / 7;
    } else {
        return timeout_turn;
    }
}


int Incephalon::computeHash(int depth, int alpha, int beta) {
    Hashe *phashe = &hashTable[zobristKey & (hashSize - 1)];
    if (phashe->key != zobristKey) return IncephalonSettings::unknown;
    if (phashe->depth < depth) return IncephalonSettings::unknown;
    if (phashe->hashf != IncephalonSettings::hash_exact) {
        if (phashe->hashf == IncephalonSettings::hash_alpha && phashe->value <= alpha) {
            return phashe->value;
        } else if (phashe->hashf == IncephalonSettings::hash_beta && phashe->value >= beta) {
            return phashe->value;
        }
    } else {
        return phashe->value;
    }
    return IncephalonSettings::unknown;
}


void Incephalon::recordMove(int depth, int value, int hashf, Vector best) {
    Hashe *phashe = &hashTable[zobristKey & (hashSize - 1)];
    Pv *e = &pvsTable[zobristKey % pvsSize];
    e->key = zobristKey;
    e->best = best;
    phashe->key = zobristKey;
    phashe->value = value;
    phashe->hashf = hashf;
    phashe->depth = depth;
}


void Incephalon::putPawn(Vector next) {
    next.x += 4, next.y += 4;
    playPawn(next);
}


Vector Incephalon::getBestMove() {
    Vector best = computeMainNode();
    best.x -= 4, best.y -= 4;
    return best;
}


Vector Incephalon::computeMainNode() {
    start = clock();
    total = 0;
    hashCount = 0;
    Vector bestMove;
    if (step == 0) {
        bestMove.x = size / 2 + 4;
        bestMove.y = size / 2 + 4;
        return bestMove;
    }
    if (step == 1 || step == 2) {
        int rx, ry;
        srand(time(NULL));
        do {
            rx = remMove[0].x + rand() % (step * 2 + 1) - step;
            ry = remMove[0].y + rand() % (step * 2 + 1) - step;
        } while (!checkCoordinates(rx, ry) || pSingleton[rx][ry].piece != Empty);
        bestMove.x = rx;
        bestMove.y = ry;
        return bestMove;
    }
    stopIncephalon = false;
    bestPoint.value = 0;
    ply = 0;
    memset(IsLose, false, sizeof(IsLose));
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = MinDepth; i <= MaxDepth && !stopIncephalon; i += 2) {
        searchDepth = i;
        //logToFile("ITERATION MAINSEARCH N°" + std::to_string(i));
        //std::cout << "MESSAGE" << " REACHED DEPTH => " << searchDepth << std::endl;
        bestPoint = browseNodes(searchDepth, -10001, 10000, &bestNode);
        if (stopIncephalon ||
            (searchDepth >= IncephalonSettings::threshold && getTime() >= 1000 && getTime() * 12 > stopClock())) {
            //std::cout << "MESSAGE" << " Incephalon STOPPED IN ORDER TO PREVENT TIMEOUT AT DEPTH " << searchDepth << std::endl;
            break;
        }

    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    auto time = std::string(std::to_string(duration));
    //logToFile(time);
    ThinkTime = getTime();
    bestMove = bestPoint.p;
    return bestMove;
}


Point Incephalon::browseNodes(int depth, int alpha, int beta, Node *pnode) {
    Point best = nodes[0];
    Node node;
    if (depth != MinDepth) {
        for (int i = 1; i < nodeCount; i++) {
            if (nodes[i].value > nodes[0].value) {
                Point temp = nodes[0];
                nodes[0] = nodes[i];
                nodes[i] = temp;
            }
        }
    } else {
        Vector moves[64];
        nodeCount = getPossibleMoves(moves);
        if (nodeCount != 1) {
            for (int i = 0; i < nodeCount; i++) {
                nodes[i].p = moves[i];
            }
        } else {
            stopIncephalon = true;
            best.p = moves[0];
            best.value = 0;
            pnode->n = 0;
            return best;
        }
    }
    int value;
    for (int i = 0; i < nodeCount; i++) {
        //logToFile("ITERATION ROOTSEARCH N°" + std::to_string(i));

        Vector p = nodes[i].p;
        if (!IsLose[p.x][p.y]) {
            node.n = 0;
            playPawn(p);
            do {
                if (i > 0 && alpha + 1 < beta) {
                    value = -computeAlphaBetaPruning(depth - 1, -alpha - 1, -alpha, &node);
                    if (value <= alpha || value >= beta) {
                        break;
                    }
                }
                value = -computeAlphaBetaPruning(depth - 1, -beta, -alpha, &node);
            } while (false);
            removeLastStep();
            nodes[i].value = value;
            if (stopIncephalon) break;
            if (value == -10000) IsLose[p.x][p.y] = true;
            if (value > alpha) {
                alpha = value;
                best.p = p;
                best.value = value;
                pnode->moves[0] = p;
                memcpy(pnode->moves + 1, node.moves, node.n * sizeof(Vector));
                pnode->n = node.n + 1;
                if (value == 10000) {
                    stopIncephalon = true;
                    return best;
                }
            }
        }
    }
    return best;
}

Vector Incephalon::getNextPossibleMove(moveList &moveList) {
    switch (moveList.phase) {
        case 0:
            moveList.phase = 1;
            Pv *e;
            e = &pvsTable[zobristKey % pvsSize];
            if (e->key == zobristKey) {
                moveList.hashMove = e->best;
                return e->best;
            }
        case 1:
            moveList.phase = 2;
            moveList.n = getPossibleMoves(moveList.moves);
            moveList.index = 0;
            if (moveList.first) {}
            else {
                for (int i = 0; i < moveList.n; i++) {
                    if (moveList.moves[i].x == moveList.hashMove.x && moveList.moves[i].y == moveList.hashMove.y) {
                        for (int j = i + 1; j < moveList.n; j++) {
                            moveList.moves[j - 1] = moveList.moves[j];
                        }
                        moveList.n--;
                        break;
                    }
                }
            }
        case 2:
            if (moveList.index < moveList.n) {
                moveList.index++;
                return moveList.moves[moveList.index - 1];
            }
        default:
            Vector p = {-1, -1};
            return p;

    }
}

int Incephalon::computeAlphaBetaPruning(int depth, int alpha, int beta, Node *pnode) {
    total++;
    static int cnt = 1000;
    if (--cnt <= 0) {
        cnt = 1000;

    }
    if ((getTime() + 100) >= stopClock()) {
        //std::cout << "MESSAGE" << " break at " << stopClock() << "with get time at " << getTime() << std::endl;
        stopIncephalon = true;
        return alpha;
    }
    if (checkWin()) {
        return -10000;
    }
    if (depth <= 0) {
        return rateMove();
    }
    int value = computeHash(depth, alpha, beta);
    if (value != IncephalonSettings::unknown) {
        hashCount++;
        return value;
    }
    Node node;
    moveList moveList;
    moveList.phase = 0;
    moveList.first = true;
    Vector p = getNextPossibleMove(moveList);
    Point best = {p, -10000};
    int hashf = IncephalonSettings::hash_alpha;
    while (p.x != -1) {
        node.n = 0;
        playPawn(p);
        do {
            if (!moveList.first && alpha + 1 < beta) {
                value = -computeAlphaBetaPruning(depth - 1, -alpha - 1, -alpha, &node);
                if (value <= alpha || value >= beta) {
                    break;
                }
            }
            //logToFile(std::to_string(getTime()));
            value = -computeAlphaBetaPruning(depth - 1, -beta, -alpha, &node);
        } while (0);
        removeLastStep();
        if (stopIncephalon) return best.value;
        if (value >= beta) {
            recordMove(depth, value, IncephalonSettings::hash_beta, p);
            return value;
        }
        if (value > best.value) {
            best.value = value;
            best.p = p;
            if (value > alpha) {
                hashf = IncephalonSettings::hash_exact;
                alpha = value;
                pnode->moves[0] = p;
                memcpy(pnode->moves + 1, node.moves, node.n * sizeof(Vector));
                pnode->n = node.n + 1;
            }
        }
        p = getNextPossibleMove(moveList);
        moveList.first = false;
    }
    recordMove(depth, best.value, hashf, best.p);
    return best.value;
}


int Incephalon::removeNodes(Vector *move, Point *cand, int Csize) {
    if (cand[0].value >= 2400) {
        move[0] = cand[0].p;
        return 1;
    }
    int moveCount = 0;
    if (cand[0].value == 1200) {
        for (int i = 0; i < Csize; i++) {
            if (cand[i].value == 1200) {
                move[moveCount] = cand[i].p;
                moveCount++;
            } else break;
        }
        for (int i = moveCount; i < Csize; ++i) {
            Singleton *p = &pSingleton[cand[i].p.x][cand[i].p.y];
            if (checkPawnType(p, myAI, IncephalonSettings::block4) ||
                checkPawnType(p, opp, IncephalonSettings::block4)) {
                move[moveCount] = cand[i].p;
                moveCount++;
                if (moveCount >= MaxMoves) {
                    break;
                }
            }
        }
    }
    return moveCount;
}


int Incephalon::getPossibleMoves(Vector *move) {
    int candCount = 0;
    int moveCount = 0;
    int value;
    for (int i = b_start; i < b_end; i++) {
        for (int j = b_start; j < b_end; j++) {
            if (pSingleton[i][j].IsCand && pSingleton[i][j].piece == Empty) {
                value = evaluateMove(&pSingleton[i][j]);
                if (value > 0) {
                    cand[candCount].p.x = i;
                    cand[candCount].p.y = j;
                    cand[candCount].value = value;
                    ++candCount;
                }
            }
        }
    }
    sort(cand, candCount);
    moveCount = removeNodes(move, cand, candCount);
    if (moveCount == 0) {
        for (int i = 0; i < candCount; i++) {
            move[i] = cand[i].p;
            moveCount++;
            if (moveCount >= MaxMoves) {
                break;
            }
        }
    }
    return moveCount;
}


void Incephalon::sort(Point *a, int n) {
    int i, j;
    Point key;
    for (i = 1; i < n; i++) {
        key = a[i];
        for (j = i; j > 0 && a[j - 1].value < key.value; j--) {
            a[j] = a[j - 1];
        }
        a[j] = key;
    }
}

int Incephalon::rateMove() {
    int whoType[8] = {0};
    int oppType[8] = {0};
    int block4_temp = 0;
    //int x = 0;
    //std::mutex m;
    //std::for_each(std::execution::par_unseq, std::begin(pSingleton), std::end(pSingleton), [&](Singleton *current) {
    //    //std::lock_guard<std::mutex> guard(m);
    //    for (int j = b_start; j < b_end; ++j) {
    //        if (current[j].IsCand && current[j].piece == Empty) {
    //            block4_temp = whoType[IncephalonSettings::block4];
    //            countPawns(&current[j], myAI, whoType);
    //            countPawns(&current[j], opp, oppType);
    //            if (whoType[IncephalonSettings::block4] - block4_temp >= 2) {
    //                whoType[IncephalonSettings::block4] -= 2;
    //                whoType[IncephalonSettings::flex4]++;
    //            }
    //        }
    //    }
    //});
    for (int i = b_start; i < b_end; ++i) {
        for (int j = b_start; j < b_end; ++j) {
            if (!pSingleton[i][j].IsCand || pSingleton[i][j].piece != Empty) continue;
            block4_temp = whoType[IncephalonSettings::block4];
            countPawns(&pSingleton[i][j], myAI, whoType);
            countPawns(&pSingleton[i][j], opp, oppType);
            if (whoType[IncephalonSettings::block4] - block4_temp >= 2) {
                whoType[IncephalonSettings::block4] -= 2;
                whoType[IncephalonSettings::flex4]++;
            }
        }
    }
    if (whoType[IncephalonSettings::win] >= 1) return 10000;
    if (oppType[IncephalonSettings::win] >= 2) return -10000;
    if (oppType[IncephalonSettings::win] == 0 && whoType[IncephalonSettings::flex4] >= 1) return 10000;
    int myScore = 0, oppScore = 0;
    for (int i = 1; i < 8; ++i) {
        myScore += whoType[i] * eval[i];
        oppScore += oppType[i] * eval[i];
    }
    return myScore * 1.2 - oppScore;
}


int Incephalon::evaluateMove(Singleton *pSingleton) {
    int score[2];
    score[myAI] = pval[pSingleton->pattern[myAI][0]]
    [pSingleton->pattern[myAI][1]][pSingleton->pattern[myAI][2]][pSingleton->pattern[myAI][3]];
    score[opp] = pval[pSingleton->pattern[opp][0]]
    [pSingleton->pattern[opp][1]][pSingleton->pattern[opp][2]][pSingleton->pattern[opp][3]];
    if (score[myAI] < 200 && score[opp] < 200) {
        return score[myAI] * 2 + score[opp];
    } else {
        return score[myAI] >= score[opp] ? score[myAI] * 2 : score[opp];
    }
}
