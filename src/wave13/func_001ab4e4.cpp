#include "game/selection.h"



extern "C" void func_001ab4e4(W6SelectionScreen *screen)
{
    SelectionInterface *interface = reinterpret_cast<SelectionInterface *>(screen);
    interface->vtable->begin(screen);
    func_001a78a4(screen, 1);
    interface->vtable->end(screen);
}
