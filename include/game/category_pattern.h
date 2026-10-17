#ifndef GAME_CATEGORY_PATTERN_H
#define GAME_CATEGORY_PATTERN_H

struct BytePattern;
struct CategoryPattern;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct BytePattern {
    unsigned char bytes[5];
};

struct CategoryPattern {
    short selection;
    short value;
    union { struct BytePattern pattern; signed char members[6]; } details;
};

#endif
