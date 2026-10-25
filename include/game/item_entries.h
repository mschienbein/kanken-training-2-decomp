#ifndef GAME_ITEM_ENTRIES_H
#define GAME_ITEM_ENTRIES_H

#include "game/runtime.h"

#include "game/item_pages.h"


struct W8EntryList {
    unsigned char reserved00[0x10];
    int count;
    struct ItemPage **entries;
};

#ifdef __cplusplus
extern "C" {
#endif
extern void *func_0019d788(unsigned size);
extern struct ItemPage *func_0015f84c(struct ItemPage *entry);
/* [INFERENCE] Candidate-only field accessors; historical identities unknown. */
int w19_item_category(const struct ItemPage *entry);
unsigned w19_item_value(const struct ItemPage *entry);
extern int func_001e5d38(const struct W8EntryList *list, int category);
#ifdef __cplusplus
}
#endif

#endif
