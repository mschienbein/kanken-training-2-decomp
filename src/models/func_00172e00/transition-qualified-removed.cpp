#include "game/transition.h"

/* [INFERENCE] A tiny completion setter may have been inlined and removed by
 * armlink. Its original identity is not established by the surviving caller. */
void func_00172e00(MenuTransitionState *screen, int completion)
{
    screen = reinterpret_cast<MenuTransitionState *>(set_completion(&screen->completion_state, completion));
    screen->modes[screen->stack_index] = 0;
    func_0017433c(0, (int)func_0017adb4() * 60);
    func_00181b68(7, 1, 1);
    func_00177b10(reinterpret_cast<CallbackTarget *>(screen), &CallbackTarget::callback5);
}
