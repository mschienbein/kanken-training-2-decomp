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
    int count;
    unsigned value10;
    signed char weight;
    unsigned char reserved15[3];
    unsigned value18;
    const unsigned char *values;
};

struct W8EntryList {
    unsigned char reserved00[0x10];
    int count;
    struct W8ItemEntry **entries;
};

/* The same saved list receiver reaches both 001730ec and 001e5d38 in each
 * observed caller (00163770, 00178154). Their entries therefore use one
 * contract; this is not an equivalence inferred from offsets alone. */

#ifdef __cplusplus
extern "C" {
#endif
extern void *func_0019d788(unsigned size);
extern struct W8ItemEntry *func_0015f84c(struct W8ItemEntry *entry);
extern int func_0015f5b0(const struct W8ItemEntry *entry);
extern int func_001743d8(const struct W8EntryList *list);
extern int func_001e5d38(const struct W8EntryList *list, int category);
/* [INFERENCE] Candidate-only field accessors; historical identities unknown. */
int w19_item_category(const struct W8ItemEntry *entry);
unsigned w19_item_value(const struct W8ItemEntry *entry);
#ifdef __cplusplus
}
#endif

#endif
