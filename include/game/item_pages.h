#ifndef GAME_ITEM_PAGES_H
#define GAME_ITEM_PAGES_H

/* Paged item lists (partial). */

struct ItemPage {
    unsigned char reserved00[0x0c];
    int count;                              /* 0x0c */
};

struct ItemPageSet {
    unsigned char reserved00[0x10];
    void *entries;                          /* 0x10 */
    struct ItemPage **pages;                /* 0x14 */
};

extern struct ItemPageSet *data_0023ecf4;
extern struct ItemPage *func_0017a4e8(struct ItemPageSet *set, int index);

#endif
