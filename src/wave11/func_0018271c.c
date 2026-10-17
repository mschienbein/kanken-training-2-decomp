#include "game/screen.h"

struct W11jPaneAlpha {
    unsigned char reserved00[0xb4];
    unsigned char alpha;
};

/* Preserve the interface used by src/wave4/func_00182718.c. */
void func_0018271c(void *object, unsigned char alpha)
{
    struct BoundDisplayObject *display = object;
    struct W11jPaneAlpha *pane =
        (struct W11jPaneAlpha *)display->bindings[0]->pane;
    pane->alpha = alpha;
}
