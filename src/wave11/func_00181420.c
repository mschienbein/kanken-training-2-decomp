#include "game/screen.h"

/* INTEGRATOR: consolidate with BoundDisplayObject in game/screen.h; this view
 * identifies the second sort link at 0xb4 and the embedded event's 0xd0 extent. */
struct ResourceEvent {
    struct DisplayObject base;
    struct DisplayBinding *bindings[5];
    int screen;
    int index;
    struct ResourceEvent *next;
    struct ResourceEvent *secondary_next;
    unsigned char reservedb8[8];
    unsigned char queued;
    unsigned char reservedc1[0xf];
};

extern struct ResourceEvent *func_00145b5c(int screen, int index);
extern void func_00181448(struct ResourceEvent *event);

void func_00181420(int screen, int index, float depth)
{
    struct ResourceEvent *event = func_00145b5c(screen, index);
    event->screen = screen;
    event->base.position[2] = depth;
    func_00181448(event);
}
