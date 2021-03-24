#ifndef PBRAIN_INCEPHALON_SETTINGS_H
#define PBRAIN_INCEPHALON_SETTINGS_H

class IncephalonSettings {
public:
   static const int win;
   static const int flex4;
   static const int block4;
   static const int flex3;
   static const int block3;
   static const int flex2;
   static const int block2;
   static const int threshold;
   static const int hash_exact;
   static const int hash_alpha;
   static const int hash_beta;
   static const int unknown;
   static const int dx[4];
   static const int dy[4];
};

const int MaxSize = 20;
const int MaxMoves = 40;
const int hashSize = 1 << 22;
const int pvsSize = 1 << 20;
const int MaxDepth = 40;
const int MinDepth = 2;

enum Pieces {
    White = 0, Black = 1, Empty = 2, Outside = 3
};


#endif //PBRAIN_INCEPHALON_SETTINGS_H
