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

/* [INFERENCE] Names record with its counted table at the observed +0x84 member. */
struct W8TextTable {
    unsigned char reserved00[0x84];
    struct TextTable names;
};

struct MenuItem {
    struct TextTable text;
    unsigned char reserved2c[0xdc];
    struct TextTable alternate_text;
    unsigned char reserved134[0x80];
    signed char link_count;
    unsigned char reserved1b5[3];
    const unsigned short *link;
    unsigned char reserved1bc[0x7c];          /* allocation extent: 0x238 */
};

static __inline const unsigned short *const *text_table_entries(const struct TextTable *table)
{
    return (const unsigned short *const *)(table + 1);
}

#ifdef __cplusplus
extern "C" {
#endif

extern int func_0017adc4(const unsigned short *text);   /* length in code units */
/* Search compares the full incoming word against zero-extended code units. */
extern const unsigned short *func_0015dd5c(const unsigned short *text, unsigned character);
extern int func_0016e5cc(const unsigned short *left, const unsigned short *right, int count);
extern const unsigned short *func_00179090(const struct MenuItem *item);
extern const unsigned short *func_001791e0(const struct TextTable *table, int index);
extern unsigned short *func_00178f28(unsigned short *destination, unsigned byte_limit,
                                    const unsigned short *source);
extern const unsigned short *func_00178f5c(const struct W8TextTable *table, int index);
/* [INFERENCE] Candidate-only item accessor identities remain unrecovered. */
int r41279_item_identifier(const struct MenuItem *item);
int r41279_item_category(const struct MenuItem *item);
/* [INFERENCE] Wide-copy identity remains address-named, not authenticated wcsncpy. */
extern unsigned short *func_0018651c(unsigned short *destination,
                                   const unsigned short *source, unsigned count);
extern unsigned func_001949e4(unsigned short *destination, int *destination_count,
                             const char *source, int *source_count);
extern unsigned func_0012e930(unsigned short *destination, int *destination_count,
                             const char *source, int *source_count, unsigned newline);
extern unsigned func_00193168(unsigned short *destination, unsigned capacity,
                             const char *source);

#ifdef __cplusplus
}
#endif

#endif
