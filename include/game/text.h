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

#ifdef __cplusplus
/* [INFERENCE] Reader identities and encoding roles; the original method-pointer
 * tables remain external data, not new method-name aliases or credited data. */
struct EncodingSource;
struct EncodingSourceVTable {
    void (*reserved00[18])(void);
    int (*encoding)(EncodingSource *source);
};
struct EncodingSource {
    const EncodingSourceVTable *vtable;
};
struct CodeUnitReader {
    typedef unsigned short (CodeUnitReader::*NextCodeUnit)();
    union Cursor {
        const void *pointer;
        const unsigned char *bytes;
        const unsigned short *words;
    } cursor;
    NextCodeUnit next;
};
struct CodeUnitReaderAlt {
    typedef unsigned short (CodeUnitReaderAlt::*NextCodeUnit)();
    union Cursor {
        const void *pointer;
        const unsigned char *bytes;
        const unsigned short *words;
    } cursor;
    NextCodeUnit next;
};
extern "C" {
extern const CodeUnitReader::NextCodeUnit data_0021c5e0[4];
extern const CodeUnitReaderAlt::NextCodeUnit data_0021c64c[4];
void func_0013156c(CodeUnitReader *reader, EncodingSource *source);
void func_001f4cf8(CodeUnitReaderAlt *reader, EncodingSource *source);
}
#endif

#endif
