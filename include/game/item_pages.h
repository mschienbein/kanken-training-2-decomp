#ifndef GAME_ITEM_PAGES_H
#define GAME_ITEM_PAGES_H

/* Paged item lists (partial). */

struct ItemPage {
    unsigned char reserved00[0x0c];
    int count;                              /* 0x0c */
    unsigned char reserved10[8];
    unsigned short **values;
    unsigned char *flags;
};

struct ItemPageSet {
    unsigned char reserved00[0x10];
    void *entries;                          /* 0x10 */
    struct ItemPage **pages;                /* 0x14 */
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct ItemPageSet *data_0023ecf4;
extern struct ItemPage *func_0017a4e8(struct ItemPageSet *set, int index);
#ifdef __cplusplus
}
#endif

#endif
