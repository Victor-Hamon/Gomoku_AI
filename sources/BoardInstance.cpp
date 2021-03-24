#include "BoardInstance.h"
#include <cstring>
#include <ctime>
#include <cstdlib>

using namespace std;

BoardInstance::BoardInstance() {
    initPawnType();
    initZobrist();
    memset(pSingleton, 0, sizeof(pSingleton));
    memset(IsLose, 0, sizeof(IsLose));
    memset(remMove, 0, sizeof(remMove));
    memset(pvsTable, 0, sizeof(pvsTable));
    memset(hashTable, 0, sizeof(hashTable));
}

BoardInstance::~BoardInstance() = default;

uint64_t BoardInstance::rand64() {
    //td::default_random_engine re(time(0));
    //td::uniform_int_distribution<int> distribution{0, RAND_MAX};
    //eturn distribution(re) ^ ((uint64_t) distribution(re) << 15) ^ ((uint64_t) distribution(re) << 30) ^ ((uint64_t) distribution(re) << 45) ^
    //      ((uint64_t) distribution(re) << 60);
    return rand() ^ ((uint64_t) rand() << 15) ^ ((uint64_t) rand() << 30) ^ ((uint64_t) rand() << 45) ^
           ((uint64_t) rand() << 60);
}

void BoardInstance::initZobrist() {
    srand(time(NULL));
    for (int i = 0; i < MaxSize + 4; i++) {
        for (int j = 0; j < MaxSize + 4; j++) {
            zobrist[0][i][j] = rand64();
            zobrist[1][i][j] = rand64();
        }
    }
}


void BoardInstance::setSize(int _size) {
    size = _size;
    b_start = 4, b_end = size + 4;
    for (int i = 0; i < MaxSize + 8; i++) {
        for (int j = 0; j < MaxSize + 8; j++) {
            pSingleton[i][j].piece = i < 4 || i >= size + 4 || j < 4 || j >= size + 4 ? Outside : Empty;
        }
    }
}

void BoardInstance::playPawn(Vector next) {
    int x = next.x;
    int y = next.y;
    ply++;
    pSingleton[x][y].piece = myAI;
    zobristKey ^= zobrist[myAI][x][y];
    myAI ^= 1;
    opp ^= 1;
    remMove[step] = next;
    step++;
    updatePawn(x, y);
    for (int i = x - 2; i <= x + 2; i++) {
        pSingleton[i][y - 2].IsCand++;
        pSingleton[i][y - 1].IsCand++;
        pSingleton[i][y].IsCand++;
        pSingleton[i][y + 1].IsCand++;
        pSingleton[i][y + 2].IsCand++;
    }
}

void BoardInstance::removeLastStep() {
    step--;
    int x = remMove[step].x;
    int y = remMove[step].y;
    ply--;
    myAI ^= 1;
    opp ^= 1;
    zobristKey ^= zobrist[myAI][x][y];
    pSingleton[x][y].piece = Empty;
    updatePawn(x, y);
    Singleton *c = &pSingleton[x][y];
    for (int i = x - 2; i <= x + 2; i++) {
        pSingleton[i][y - 2].IsCand--;
        pSingleton[i][y - 1].IsCand--;
        pSingleton[i][y].IsCand--;
        pSingleton[i][y + 1].IsCand--;
        pSingleton[i][y + 2].IsCand--;
    }
}

void BoardInstance::restart() {
    memset(pvsTable, 0, sizeof(pvsTable));
    memset(hashTable, 0, sizeof(hashTable));
    while (step) {
        removeLastStep();
    }
}

void BoardInstance::updatePawn(int x, int y) {
    int a, b;
    int key;
    for (int i = 0; i < 4; ++i) {
        a = x + IncephalonSettings::dx[i];
        b = y + IncephalonSettings::dy[i];
        for (int j = 0; j < 4 && checkCoordinates(a, b); a += IncephalonSettings::dx[i],
                b += IncephalonSettings::dy[i], ++j) {
            key = getKey(a, b, i);
            pSingleton[a][b].pattern[0][i] = patternTable[key][0];
            pSingleton[a][b].pattern[1][i] = patternTable[key][1];
        }
        a = x - IncephalonSettings::dx[i];
        b = y - IncephalonSettings::dy[i];
        for (int k = 0; k < 4 && checkCoordinates(a, b); a -= IncephalonSettings::dx[i],
                b -= IncephalonSettings::dy[i], ++k) {
            key = getKey(a, b, i);
            pSingleton[a][b].pattern[0][i] = patternTable[key][0];
            pSingleton[a][b].pattern[1][i] = patternTable[key][1];
        }
    }
}

int BoardInstance::getKey(int x, int y, int i) {
    const int stepX = IncephalonSettings::dx[i];
    const int stepY = IncephalonSettings::dy[i];
    return (pSingleton[x - stepX * 4][y - stepY * 4].piece)
           ^ (pSingleton[x - stepX * 3][y - stepY * 3].piece << 2)
           ^ (pSingleton[x - stepX * 2][y - stepY * 2].piece << 4)
           ^ (pSingleton[x - stepX * 1][y - stepY * 1].piece << 6)
           ^ (pSingleton[x + stepX * 1][y + stepY * 1].piece << 8)
           ^ (pSingleton[x + stepX * 2][y + stepY * 2].piece << 10)
           ^ (pSingleton[x + stepX * 3][y + stepY * 3].piece << 12)
           ^ (pSingleton[x + stepX * 4][y + stepY * 4].piece << 14);
}

int BoardInstance::getNodeSpec(int role, int key) {
    int node_left[9];
    int node_right[9];
    for (int i = 0; i < 9; i++) {
        if (i == 4) {
            node_left[i] = role;
            node_right[i] = role;
        } else {
            node_left[i] = key & 3;
            node_right[8 - i] = key & 3;
            key >>= 2;
        }
    }
    int p1 = computeShortNode(node_left);
    int p2 = computeShortNode(node_right);
    if (p1 == IncephalonSettings::block3 && p2 == IncephalonSettings::block3) {
        return checkFlex3(node_left);
    } else if (p1 == IncephalonSettings::block4 && p2 == IncephalonSettings::block4) {
        return checkFlex4(node_left);
    } else {
        return p1 > p2 ? p1 : p2;
    }
}

int BoardInstance::checkFlex3(int *node) {
    int role = node[4];
    int type;
    for (int i = 0; i < 9; i++) {
        if (node[i] == Empty) {
            node[i] = role;
            type = checkFlex4(node);
            node[i] = Empty;
            if (type == IncephalonSettings::flex4)
                return IncephalonSettings::flex3;
        }
    }
    return IncephalonSettings::block3;
}

int BoardInstance::checkFlex4(const int *node) {
    int i, j, count;

    int five = 0;
    int role = node[4];
    for (i = 0; i < 9; i++) {
        if (node[i] == Empty) {
            count = 0;
            for (j = i - 1; j >= 0 && node[j] == role; j--)
                count++;
            for (j = i + 1; j <= 8 && node[j] == role; j++)
                count++;
            if (count >= 4)
                five++;
        }
    }
    return five >= 2 ? IncephalonSettings::flex4 : IncephalonSettings::block4;
}

int BoardInstance::computeShortNode(const int *node) {
    int kong = 0, block = 0;
    int len = 1, len2 = 1, count = 1;
    int k;

    int myAI = node[4];
    for (k = 5; k <= 8; k++) {
        if (node[k] == myAI) {
            if (kong + count > 4)
                break;
            ++count;
            ++len;
            len2 = kong + count;
        } else if (node[k] == Empty) {
            ++len;
            ++kong;
        } else {
            if (node[k - 1] == myAI) {
                block++;
            }
            break;
        }
    }
    kong = len2 - count;
    for (k = 3; k >= 0; k--) {
        if (node[k] == myAI) {
            if (kong + count > 4)
                break;
            ++count;
            ++len;
            len2 = kong + count;
        } else if (node[k] == Empty) {
            ++len;
            ++kong;
        } else {
            if (node[k + 1] == myAI) {
                block++;
            }
            break;
        }
    }
    return typeTable[len][len2][count][block];
}

int BoardInstance::getHeuristicAssist(int len, int len2, int count, int block) {
    if (len >= 5 && count > 1) {
        if (count == 5) {
            return IncephalonSettings::win;
        }
        if (len > 5 && len2 < 5 && block == 0) {
            switch (count) {
                case 2:
                    return IncephalonSettings::flex2;
                case 3:
                    return IncephalonSettings::flex3;
                case 4:
                    return IncephalonSettings::flex4;
            }
        } else {
            switch (count) {
                case 2:
                    return IncephalonSettings::block2;
                case 3:
                    return IncephalonSettings::block3;
                case 4:
                    return IncephalonSettings::block4;
            }
        }
    }
    return 0;
}

int BoardInstance::getPval(int a, int b, int c, int d) {
    int type[8] = {0};
    type[a]++;
    type[b]++;
    type[c]++;
    type[d]++;
    if (type[IncephalonSettings::win] > 0) return 5000;
    if (type[IncephalonSettings::flex4] > 0 || type[IncephalonSettings::block4] > 1) return 1200;
    if (type[IncephalonSettings::block4] > 0 && type[IncephalonSettings::flex3] > 0) return 1000;
    if (type[IncephalonSettings::flex3] > 1) return 200;
    int value[6] = {0, 2, 5, 5, 12, 12};
    int score = 0;
    for (int i = 1; i <= IncephalonSettings::block4; i++)
        score += value[i] * type[i];
    return score;
}

void BoardInstance::initPawnType() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 6; ++j) {
            for (int k = 0; k < 6; ++k) {
                for (int l = 0; l < 3; ++l) {
                    typeTable[i][j][k][l] = getHeuristicAssist(i, j, k, l);
                }
            }
        }
    }
    for (int key = 0; key < 65536; key++) {
        patternTable[key][0] = getNodeSpec(0, key);
        patternTable[key][1] = getNodeSpec(1, key);
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            for (int k = 0; k < 8; ++k) {
                for (int l = 0; l < 8; ++l) {
                    pval[i][j][k][l] = getPval(i, j, k, l);
                }
            }
        }
    }
}

bool BoardInstance::checkCoordinates(int x, int y) {
    return pSingleton[x][y].piece != Outside;
}

Singleton *BoardInstance::getLastStep() {
    return &pSingleton[remMove[step - 1].x][remMove[step - 1].y];
}

void BoardInstance::countPawns(Singleton *c, int role, int *type) {
    ++type[c->pattern[role][0]];
    ++type[c->pattern[role][1]];
    ++type[c->pattern[role][2]];
    ++type[c->pattern[role][3]];
}


bool BoardInstance::checkPawnType(Singleton *c, int role, int type) {
    return c->pattern[role][0] == type || c->pattern[role][1] == type || c->pattern[role][2] == type ||
           c->pattern[role][3] == type;
}

bool BoardInstance::checkWin() {
    Singleton *c = getLastStep();

    return c->pattern[opp][0] == IncephalonSettings::win || c->pattern[opp][1] == IncephalonSettings::win || c->pattern[opp][2] == IncephalonSettings::win ||
           c->pattern[opp][3] == IncephalonSettings::win;
}
