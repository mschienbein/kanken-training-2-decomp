#include <stddef.h>
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
extern void func_0017a43c(struct MessageLayout *layout, const unsigned short *left,
                         const unsigned short *right, int index);
/* INTEGRATOR: differs from src/wave10/func_00170db4.c: the root object is a
 * MessageLayout whose entries begin with the referenced display object. */
extern void func_00170dc0(struct MessageLayout *layout, int index, int enabled);

void func_00177520(struct MessageLayout *layout)
{
    func_0017a43c(layout, (const unsigned short *)L"\x3000", (const unsigned short *)L"\x3000", 0);
    func_0017a43c(layout, (const unsigned short *)L"\x3000", (const unsigned short *)L"\x3000", 1);
    func_00170dc0(layout, 1, 0);
}
