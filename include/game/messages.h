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
    unsigned char reserved14[8];
    /* [INFERENCE] The shared +0x78 receiver in 001b8908 reaches both the text
     * assignment family and 00177b84; the latter updates this word at +0x1c.
     * Its preceding bytes are not seven independently evidenced handle slots. */
    int activity_state;
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
extern void func_00177198(struct MessageLayout *layout, const struct W11TextResource *archive, int index);
extern void func_00177520(struct MessageLayout *layout);
extern void func_00177598(struct MessageLayout *layout, float x, float y);
extern void func_001ad3c8(struct MessageLayout *layout, int index,
                          const unsigned short *left, const unsigned short *right);
extern void func_001ad5a8(struct MessageLayout *layout, int index);
/* These helpers receive the same layout object, not an unrelated handle-array
 * owner. The original forwards its first address, without a receiver cast. */
extern void func_00170dc0(struct MessageLayout *layout, int index, int enabled);
extern void func_00170db4(struct MessageLayout *layout, int enabled);
extern void func_00177b84(struct MessageLayout *layout, int enabled);
extern void func_001ad6d0(struct MessageLayout *layout, int first, int second);
extern void func_001ad6ec(struct MessageLayout *layout, struct WidgetPosition position);
extern const unsigned short data_002276bc[];
#ifdef __cplusplus
}
#endif


#endif
