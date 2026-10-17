#include "game/screen.h"

struct MessageEntry {
    struct DisplayObject *object;
    unsigned short *output;
    float width;
    float spacing;
    float x, y;
    float animation_x, animation_y;
};
struct MessageLayout {
    struct MessageEntry *entries[3];
    float x, y;
    unsigned char reserved14[0xc];
    int lines;
    unsigned char reserved24[4];
    unsigned short *text;
};
extern void func_001ad3c8(struct MessageLayout *layout, int index,
                          const unsigned short *left, const unsigned short *right);
extern void func_001ad5a8(struct MessageLayout *layout, int index);

static void set_animation(struct MessageEntry *entry, float x, float y)
{
    if (entry != 0 && entry->object != 0) {
        entry->animation_x = x;
        entry->animation_y = y;
    }
}

void func_0017a43c(struct MessageLayout *layout, const unsigned short *left,
                   const unsigned short *right, int index)
{
    struct MessageEntry *entry;
    func_001ad3c8(layout, index, left, right);
    if (layout->text != 0) {
        func_001ad5a8(layout, index);
        entry = layout->entries[index];
        set_animation(entry, 0.0f, 0.0f);
    }
}
