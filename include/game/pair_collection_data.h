#ifndef GAME_PAIR_COLLECTION_DATA_H
#define GAME_PAIR_COLLECTION_DATA_H
#include "game/menu_collections.h"

struct PairHandle;
/* Actual offset-zero value member of the native PairCollection below. The
 * 001b6400 singleton owns ten eight-byte references, then the transition tail.
 * C R1 accessors consume this real member, not a re-declared C++ class view. */
struct PairCollectionData {
    struct MaskHeader dimensions;
    unsigned mask;
    struct PairHandle *entries[10];
    unsigned char active, paused, reserved36[2];
    float elapsed, duration;
    int mode;
};
#endif
