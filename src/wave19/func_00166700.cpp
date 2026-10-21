#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/screen_transition.h"



extern "C" void func_00166700(CategoryScene *state, int mode)
{
    state->index = mode;
    func_00166ae8(state, 0, -1);
    if (mode == 4)
        func_0017433c(0, (int)func_0017adb4() * 60);
    func_00181b68(7, 1, 1);
    if (state->mode != 1)
        func_00172d48(-1);
    func_00177b10(reinterpret_cast<MenuScene *>(state), data_00218868);
}
