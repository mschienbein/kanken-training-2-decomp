#include "game/layout.h"
template <typename T>
static inline T maximum(T left, T right)
{
    if (left < right)
        left = right;
    return left;
}

extern "C" void func_001c5918(struct Pane *pane, int extent)
{
    pane->extent = maximum(extent, 0);
    func_001c5930(pane->style, extent);
}
