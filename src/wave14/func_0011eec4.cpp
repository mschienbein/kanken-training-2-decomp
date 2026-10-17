#include "game/layout_draw.h"
#include "game/matrix.h"
#include "game/screen.h"


#include <float.h>

LayoutDrawState::LayoutDrawState()
    : max_depth(FLT_MAX), value50(0.0f), value54(0.0f), kind(4), flags(0),
      table(data_002438dc)
{
}
