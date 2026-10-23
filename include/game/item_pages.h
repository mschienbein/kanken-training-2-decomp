#ifndef GAME_ITEM_PAGES_H
#define GAME_ITEM_PAGES_H

/* Paged item lists (partial). */
struct W11SingleState;

struct ItemPage {
    unsigned char reserved00[6];
    signed char filter_kind;                /* 0x06; signed filtered-count property */
    unsigned char reserved07[5];
    int count;                              /* 0x0c */
    unsigned char reserved10[8];
    unsigned short **values;
    unsigned char *flags;
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
extern int func_00173078(const struct ItemPageSet *set, int kind);
/* [INFERENCE] R1 removes this supplied getter; historical identity is unknown. */
extern signed char item_page_kind(const struct ItemPage *page);
#ifdef __cplusplus
}
#endif




#endif
