#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/screen_transition.h"



extern "C" void func_00166700(ScreenTransition *state, int mode)
{
    state->mode = mode;
    func_00166ae8(state, 0, -1);
    if (mode == 4)
        func_0017433c(0, (int)func_0017adb4() * 60);
    func_00181b68(7, 1, 1);
    if (state->notification_mode != 1)
        func_00172d48(-1);
    func_00177b10(reinterpret_cast<CallbackTarget *>(state), data_00218868);
}
