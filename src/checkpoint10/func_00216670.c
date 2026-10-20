#include "game/layout_draw.h"
#include "game/functions.h"

float func_00216670(struct LayoutDrawState *writer,
                    const unsigned short *text, int length)
{
    struct LayoutDrawState copy;
    float width;
    func_00192720(&copy, writer, sizeof(copy));
    width = func_002166cc(&copy, text, length);
    writer->base.position = copy.base.position;
    /* [INFERENCE] callee identity: inlined by armlink */
    func_00125b54(&copy);
    return width;
}
