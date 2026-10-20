#include <preinclude.h>
#include "nw/lyt/lyt_Picture.h"
#include "game/screen.h"

extern "C" void func_0018271c(DisplayObject *object, unsigned char alpha)
{
    display_pane_attachment(object, 0)->picture->SetAlpha(alpha);
}
