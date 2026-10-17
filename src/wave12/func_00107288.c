#include "game/screen.h"
#include "game/audio.h"

void func_00107288(void)
{
    if (func_00115138()) {
        struct DisplayObject *object = data_0023ee9c.object;
        if (data_0023ee9c.available == 0)
            goto check_active;
        if (data_0023ee9c.active == 0) {
            data_0023ee9c.active = 1;
            if (object != 0) {
                func_00195a08(object, 0, 1);
                func_00195900(object, 0, "HomeNixSign_InOut", 0);
                return;
            }
        }
        goto check_animation;
    }
check_active:
    if (data_0023ee9c.active == 0)
        return;
check_animation:
    {
        struct DisplayObject *object = data_0023ee9c.object;
        if (object != 0) {
            if (func_00195874(object, 0) == 0) {
                func_00195a08(object, 0, 0);
                data_0023ee9c.active = 0;
            }
        } else {
            data_0023ee9c.active = 0;
        }
    }
}
