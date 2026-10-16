#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

/* Menu screen with widget slots (partial). Offsets are fixed by the original code. */

struct Widget;

/* Named entries selected by index; `kind` 1 means `names` is valid. */
struct NameTable {
    unsigned int reserved00;
    int kind;                               /* 0x04 */
    unsigned char reserved08[0xc];
    int count;                              /* 0x14 */
    const char **names;                     /* 0x18 */
};

struct WidgetSlot {
    struct Widget *widget;
    short label_index;
    unsigned char enabled;
};

struct Screen {
    unsigned char reserved00[0x0c];
    struct WidgetSlot *slots[0x36];         /* 0x00c; highest index used: 0x35 */
    unsigned char reservede4[4];
    int selected;                           /* 0x0e8 */
    int count;                              /* 0x0ec */
    int mode;                               /* 0x0f0 */
    unsigned char reservedf4[0xf0];
    const struct NameTable *const *labels;  /* 0x1e4 */
};

extern void func_0016e1d0(struct Screen *screen, int index, int enabled);
extern void func_0017a968(struct Widget *widget, const struct NameTable *table, int index, int mode);
extern void func_001b7c9c(struct Screen *screen, int index, int value, const struct NameTable *const *labels);

#endif
