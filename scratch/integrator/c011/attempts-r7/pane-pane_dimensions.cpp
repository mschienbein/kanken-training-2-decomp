#include <cstring>
#include "game/pane_tree.h"

extern "C" void func_0016f4ec(const DisplayData *object, float *width, float *height)
{
    DisplayPart *const *parts = reinterpret_cast<DisplayPart *const *>(object + 1);
    const nw::lyt::Pane *root = parts[0]->container->root;
    const nw::lyt::Pane *pane = root->GetFirstChild();
    if (pane) {
        nw::lyt::Size size;
        std::memcpy(&size, &pane->GetSize(), sizeof size);
        std::memcpy(width, &size.width, sizeof *width);
        std::memcpy(height, &size.height, sizeof *height);
    } else {
        *width = 0.0f;
        *height = 0.0f;
    }
}
