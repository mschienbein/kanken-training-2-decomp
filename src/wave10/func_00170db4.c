#include "game/screen.h"

extern void func_00170dc0(struct DisplayHandle **handles, int index, int enabled);

void func_00170db4(struct DisplayHandle **handles, int enabled)
{
    func_00170dc0(handles, 0, enabled);
}
