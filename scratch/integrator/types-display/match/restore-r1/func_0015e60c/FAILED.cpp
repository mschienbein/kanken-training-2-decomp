#include <preinclude.h>
#include <string.h>
#include "game/exp_heap.h"
#include "game/screen.h"
#include "nw/lyt/lyt_TextBox.h"

/* D08: the native TextBox font size is a two-float value at +0xe4. */
extern "C" void func_0015e60c(ResourceEntry *entry, float *width, float *height)
{
    nw::lyt::Size size = entry->object->GetFontSize();
    memcpy(width, &size.width, sizeof *width);
    memcpy(height, &size.height, sizeof *height);
}
