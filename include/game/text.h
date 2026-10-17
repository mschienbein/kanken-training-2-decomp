#ifndef GAME_TEXT_H
#define GAME_TEXT_H

/* UTF-16 text helpers. */

/* Fixed prefix; the counted UTF-16 pointer vector follows at 0x2c. */
struct TextTable {
    unsigned reserved00[4];
    unsigned char reserved10;
    signed char kind;
    unsigned char reserved12[0x0e];
    int action_first;
    int action_second;
    signed char count;
    unsigned char reserved29[3];
};

struct MenuItem {
    struct TextTable text;
    unsigned char reserved2c[0xdc];
    struct TextTable alternate_text;
    unsigned char reserved134[0x80];
    signed char link_count;
    unsigned char reserved1b5[3];
    const unsigned short *link;
};

static __inline const unsigned short *const *text_table_entries(const struct TextTable *table)
{
    return (const unsigned short *const *)(table + 1);
}

#ifdef __cplusplus
extern "C" {
#endif

extern int func_0017adc4(const unsigned short *text);   /* length in code units */
extern const unsigned short *func_00179090(const struct MenuItem *item);
extern const unsigned short *func_001791e0(const struct TextTable *table, int index);
extern unsigned short *func_00178f28(unsigned short *destination, unsigned byte_limit,
                                    const unsigned short *source);

#ifdef __cplusplus
}
#endif

#endif
