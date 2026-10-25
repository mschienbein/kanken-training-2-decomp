#include "game/display_initializer.h"

void func_001456e4(void)
{
    struct DisplayObject *object = func_00181588(0, 0, 0x20, 0);
    if (object != 0) {
        struct BoundDisplayObject *display = (struct BoundDisplayObject *)object;
        func_00181520(object, 0, 1, 0, "HomeNixSign", 0);
        func_001814e8(object, 0, 1, 0, "HomeNixSign_InOut");
        func_00195900(object, 0, "HomeNixSign_InOut", 1);
        /* [INFERENCE] Supplied natural setter; exact bytes do not establish
         * that the original source called this helper. */
        w16_set_screen(display, 0, 4);
        display->index = 3;
        func_00181700(display, 0.0f, 0.0f, 5.0f);
        func_00181448(display);
        func_00195a08(object, 0, 0);
        data_0023ee9c.object = object;
    }
}
