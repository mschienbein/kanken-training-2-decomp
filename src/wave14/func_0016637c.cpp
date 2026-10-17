#include "game/layout_draw.h"
#include "game/matrix.h"
#include "game/screen.h"



extern "C" WidgetPosition func_0016637c(const LayoutDrawPane *pane)
{
    float x = 0.0f;
    float y = 0.0f;
    unsigned char origin = pane->origin & 15;
    unsigned char horizontal = origin % 3;
    if (horizontal == 1)
        x = pane->size.x * -0.5f;
    else if (horizontal == 2)
        x = -pane->size.x;
    unsigned char vertical = origin / 3;
    if (vertical == 1)
        y = pane->size.y * 0.5f;
    else if (vertical == 2)
        y = pane->size.y;
    WidgetPosition result = {x, y};
    return result;
}
