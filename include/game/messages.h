#ifndef GAME_MESSAGES_H
#define GAME_MESSAGES_H
#include "game/screen.h"
struct MessageEntry {
    struct DisplayObject *object;
    unsigned short *output;
    float width, spacing, x, y, animation_x, animation_y;
};
struct MessageLayout {
    struct MessageEntry *entries[3];
    float x, y;
    unsigned char reserved14[0x0c];
    int lines;
    unsigned char reserved24[4];
    unsigned short *text;
};
struct W11TextEntry {
    int text_offset;
    unsigned reserved04;
    int attributes_offset;
    unsigned reserved0c;
};
struct W11TextResource { unsigned char reserved00[0x10]; const struct W11TextEntry *entries; };
/* The prefix word is skipped by both indexed text-table consumers. */
struct CategoryValues {
    unsigned reserved00;
#ifndef __cplusplus
    const unsigned short *values[];
#endif
};
#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned short *func_00170c00(const struct W11TextResource *resource, int index);
extern const struct CategoryValues *bss_00290ec4[];
extern int func_001dd3a4(int category, int kind);
/* [INFERENCE] The secondary relative pointer is forwarded through the same
 * UTF-16 pointer contract as MessageLayout's existing assignment routine;
 * its author-proposed attribute semantics are not independently recovered. */
extern const unsigned short *func_001dcbd4(const struct W11TextResource *resource, int index);
extern void func_0017a43c(struct MessageLayout *layout, const unsigned short *left,
                          const unsigned short *right, int index);
extern float func_0017a424(const struct MessageLayout *layout);
extern const unsigned short data_002276bc[];
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern float func_0017a424(const struct MessageLayout *layout);
extern void func_00177198(struct MessageLayout *layout, const struct W11TextResource *archive, int index);
#ifdef __cplusplus
}
#endif

#endif
