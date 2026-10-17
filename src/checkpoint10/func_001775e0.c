#include "game/messages.h"
#include "game/resource_tables.h"

/* INTEGRATOR: refine MessageLayout's reserved14/reserved24 fields and append
 * decoded in game/messages.h. This view preserves every canonical field. */
struct ResourceMessageLayout {
    struct MessageEntry *entries[3];
    float x, y;
    int resource_index;
    int texture;
    int state;
    int lines;
    unsigned char loaded;
    unsigned char reserved25[3];
    unsigned short *text;
    unsigned char *decoded;
};
extern unsigned char *func_0015d704(unsigned char *destination, const unsigned char *source, unsigned count);
/* INTEGRATOR: differs from earlier two-input scratch declarations.
 * Both 0x176c6c and 0x1775e0 construct the payload count in the third core
 * argument before either decoding branch. [INFERENCE] Treat it as an unused
 * decoder input; bytes cannot distinguish that formal argument from eager
 * shared header parsing. Reconcile the decoder declaration before adoption. */
extern int func_00192c20(const unsigned char *source, unsigned char *destination, unsigned count);
extern void func_00180314(int texture);
extern int func_0017fe98(int slot, const void *data, int format, int options);
extern void func_0019d95c(uintptr_t resource, int index, int mode);
extern void func_001e5b30(struct DisplayObject *object, int texture, int screen,
                          int active, int index, float x, float y, float z);
extern void func_00170dc0(struct DisplayHandle **handles, int index, int enabled);

static void set_animation(struct MessageEntry *entry, float x, float y)
{
    if (entry != 0 && entry->object != 0) {
        entry->animation_x = x;
        entry->animation_y = y;
    }
}

void func_001775e0(struct MessageLayout *message)
{
    struct ResourceMessageLayout *layout = (struct ResourceMessageLayout *)message;
    unsigned char *source;
    struct MessageEntry *entry;
    layout->state = 1;
    source = func_001971a0(14, layout->resource_index);
    if (source[0] == 'E' && source[1] == 'C' && source[2] == 'D') {
        unsigned count = (unsigned)source[13] * 65536 + (unsigned)source[14] * 256 + source[15];
        if (source[3] != 0) {
            func_00192c20(source, layout->decoded, count);
        } else {
            func_0015d704(layout->decoded, source + 16, count);
        }
    }
    if (layout->texture != -1) {
        func_00180314(layout->texture);
        layout->texture = -1;
    }
    layout->texture = func_0017fe98(layout->texture, layout->decoded, 0, 1);
    func_0019d95c(14, layout->resource_index, 0);
    entry = layout->entries[2];
    if (entry != 0 && entry->object != 0)
        func_001e5b30(entry->object, layout->texture, 3, 1, 2, 0.0f, 0.0f, 2.0f);
    func_00170dc0((struct DisplayHandle **)layout->entries, 2, 1);
    set_animation(layout->entries[2], 0.0f, 0.0f);
}
