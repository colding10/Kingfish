#include "board/bitboard.h"
#include "uci.h"

int main() {
    BBS::initLeaperAttacks();
    // // blocker bitboard
    // Bitboard block = 0ULL;

    // // init blockers
    // set_bit(block, SQ_D7);
    // set_bit(block, SQ_B4);
    // set_bit(block, SQ_D2);
    // set_bit(block, SQ_F4);

    // BBS::printBitboard(block);
    // BBS::printBitboard(BBS::rookAttacks(SQ_D4, block));

    // return 0;
    uciMainLoop();
}