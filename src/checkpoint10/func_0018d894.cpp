#include "game/matrix.h"

struct W15iRenderer;
struct W15iRendererVtable {
    unsigned char reserved00[0x10];
    unsigned char *(*begin)(W15iRenderer *, unsigned char *, unsigned flags);
    unsigned reserved14;
    unsigned char *(*end)(W15iRenderer *, unsigned char *, unsigned flags);
};
struct W15iRenderer {
    const W15iRendererVtable *vtable;
};

/* INTEGRATOR: move to include/game/render_commands.h */
extern "C" {
extern unsigned char *func_0018d8d4(void);
extern void func_0018d8c4(unsigned char *cursor);
}

static unsigned char *end_render(W15iRenderer &renderer, unsigned char *cursor,
                                  unsigned flags)
{
    return renderer.vtable->end(&renderer, cursor, flags);
}

/* Callers supply the render-info argument even though this operation ignores it. */
extern "C" void func_0018d894(W15iRenderer *renderer, MatrixObject *,
                                      unsigned flags)
{
    unsigned char *cursor = func_0018d8d4();
    cursor = end_render(*renderer, cursor, flags);
    func_0018d8c4(cursor);
}
