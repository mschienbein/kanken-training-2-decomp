#ifndef GAME_ITEM_ENTRIES_H
#define GAME_ITEM_ENTRIES_H

#include "game/runtime.h"

struct W8ItemEntry {
    const unsigned *vtable;
    short index;
    signed char first;
    signed char second;
    signed char third;
    unsigned char reserved09[3];
    unsigned value0c;
    unsigned value10;
    unsigned char flag14;
    unsigned char reserved15[3];
    unsigned value18;
    unsigned value1c;
};

struct W8EntryList {
    unsigned char reserved00[0x10];
    int count;
    struct W8ItemEntry **entries;
};

#ifdef __cplusplus
extern "C" {
#endif
extern void *func_0019d788(unsigned size);
extern struct W8ItemEntry *func_0015f84c(struct W8ItemEntry *entry);
/* [INFERENCE] Candidate-only field accessors; historical identities unknown. */
int w19_item_category(const struct W8ItemEntry *entry);
unsigned w19_item_value(const struct W8ItemEntry *entry);
#ifdef __cplusplus
}
#endif

#endif
