#ifndef GAME_ITEM_PAGES_H
#define GAME_ITEM_PAGES_H

/* Paged item lists. Child layout is shared with the typed glyph initializer. */
#include "game/text.h"
struct W11SingleState;

struct ItemPage {
    unsigned char reserved00[4];
    short group;
    signed char kind;
    signed char configured;
    signed char cached_kind;                /* 0x08; getter compares signed kinds */
    unsigned char reserved09[3];
    int count;                              /* 0x0c */
    struct MenuItem **objects;
    signed char limit;
    unsigned char reserved15[3];
    unsigned short **values;                /* 0x18; eight code units per row */
    unsigned char *flags;                   /* 0x1c */
};

struct ItemPageSet {
    unsigned char kind;
    signed char variant;
    signed char group;
    unsigned char reserved03;
    int index;
    short metric8;
    short metrica;
    unsigned char reserved0c[4];
    int count;                          /* 0x10 */
    struct ItemPage **pages;                /* 0x14 */
    /* [INFERENCE] The original configuration call follows this same +0x18
     * content binding; this is a partial receiver view, not a complete class. */
    struct W11SingleState *content;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct ItemPageSet *data_0023ecf4;
extern int func_0015f6ec(const struct ItemPage *page);
extern struct ItemPage *func_0017a4e8(struct ItemPageSet *set, int index);
extern void func_001719f4(struct ItemPageSet *set, unsigned char kind, int format,
                        int category, int index, int enabled);
extern void func_0015f724(struct ItemPage *page, int index, int column, int value);
extern void func_0015f840(struct ItemPage *page, int index, unsigned char flags);
#ifdef __cplusplus
}
#endif




#endif
