#include "game/drawing_screen.h"
/* [INFERENCE] The processing-state getter may have been armlink-inlined and removed;
 * the exact caller does not recover its historical identity or outlining. */






extern "C" void func_001bd68c(L035DrawingScreen *screen)
{
    switch (func_0017710c(reinterpret_cast<AnimationOwner *>(screen->list), 2)) {
    case 0:
        if (func_0016e03c(screen->panel, 0)) {
            func_0016dbc4(screen->input);
            func_0016d4f8(screen->input);
            screen->set_state(5);
        }
        return;
    case 1:
        if (screen->phase != 2) {
            screen->selected = func_00179ed4(screen->panel, 1);
            func_0016dc50(screen->panel, screen->selected);
        }
        screen->phase = 0;
        break;
    case 2:
        func_001770cc(reinterpret_cast<AnimationOwner *>(screen->list), 0, 0);
        func_001770cc(reinterpret_cast<AnimationOwner *>(screen->list), 1, 0);
        func_001770cc(reinterpret_cast<AnimationOwner *>(screen->list), 2, 0);
        func_001780c4(screen->list, 2, 0, 0);
        func_001780c4(screen->list, 2, 1, 0);
        screen->set_state(1);
        func_0017433c(0, (int)func_0017adb4() * 60);
        func_00181b68(7, 1, 1);
        func_00177b10(reinterpret_cast<CallbackTarget *>(screen), data_00218498);
        return;
    }

    switch (screen->phase) {
    case 0:
        func_00178e98(screen->list, 2);
        if (func_0016e03c(screen->panel, 0)) {
            func_001780c4(screen->list, 2, 0, 1);
            func_001780c4(screen->list, 2, 1, 1);
begin_drawing:
            func_0016dbc4(screen->input);
            screen->phase = 1;
        } else {
            func_001780c4(screen->list, 2, 0, 0);
            func_001780c4(screen->list, 2, 1, 0);
            goto begin_drawing;
        }
        break;
    case 1:
        if (r41262_drawing_state(screen->input) == 2) {
            screen->phase = 2;
            func_001780c4(screen->list, 2, 1, 1);
        }
        break;
    case 2:
        if (func_0016d2fc(screen->input)) {
            screen->recognized = func_0016d2dc(screen->input);
            if (screen->recognized) {
                func_0016e61c(screen->panel, screen->selected, screen->recognized);
                if (++screen->selected >= screen->count)
                    --screen->selected;
                screen->selected = func_0016dc50(screen->panel, screen->selected);
            }
            screen->phase = 0;
        }
        break;
    }
    func_0016d038(screen->input);
}
