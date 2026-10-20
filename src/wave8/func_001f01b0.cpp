#include <preinclude.h>
#include <nw/lyt/lyt_Pane.h>

namespace nw { namespace lyt {
/* Original native Pane-family vtables independently anchor this +0x68 slot.
 * The base pane emits no uniform commands and preserves the byte cursor. */
unsigned char* Pane::MakeUniformDataSelf(unsigned char* cursor,
    LayoutDrawInfo* info, LayoutRenderContext* renderer) const
{
    return cursor;
}
} }
