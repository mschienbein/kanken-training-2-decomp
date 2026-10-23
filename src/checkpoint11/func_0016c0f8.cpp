#include "game/screen_actions.h"


static void update_action_state(ScreenActionOwner *owner, int index, bool disabled)
{
    func_001c3988(owner->handles, index, disabled ^ 1);
}

void func_0016c0f8(ScreenActionOwner *owner, const ScreenAction *action)
{
    for (;;) {
        if (action->index == -1)
            return;
        func_0016c358(owner->handles, action->index, 1, action->position);
        if (action->index == 15)
            update_action_state(owner, action->index, (owner->flags & 1) != 0);
        ++action;
    }
}
