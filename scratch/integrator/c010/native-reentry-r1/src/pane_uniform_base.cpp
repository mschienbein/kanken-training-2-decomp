#include <preinclude.h>
#include <nw/lyt/lyt_Pane.h>

namespace nw { namespace lyt {
/* The original +0x68 slot in the independently anchored native Pane vtable
 * emits no uniform commands and preserves the incoming byte cursor. */
unsigned char* Pane::MakeUniformDataSelf(unsigned char* cursor,
    LayoutDrawInfo* info, LayoutRenderContext* renderer) const
{
    return cursor;
}
} }
