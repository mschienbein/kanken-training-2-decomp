#include "game/matrix.h"

#include "game/command_buffer.h"

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
