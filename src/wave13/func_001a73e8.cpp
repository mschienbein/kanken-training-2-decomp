#include "game/selection.h"



extern "C" void func_001a73e8(W6SelectionScreen *screen)
{
    SelectionInterface *interface = reinterpret_cast<SelectionInterface *>(screen);
    interface->vtable->begin(screen);
    func_00171a98(screen, 1);
    interface->vtable->end(screen);
}
