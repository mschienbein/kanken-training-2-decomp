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
#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned short *func_00170c00(const struct W11TextResource *resource, int index);
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
#endif
