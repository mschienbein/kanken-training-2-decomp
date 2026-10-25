#include "game/render_binding.h"

void func_001d2204(RenderCompleteLifetime *object)
{
    object->~RenderCompleteLifetime();
    func_001852f4(object);
}
