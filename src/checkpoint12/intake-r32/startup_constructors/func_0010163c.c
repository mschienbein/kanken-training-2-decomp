#include "game/service_state.h"

/* [INFERENCE] Descriptive independently witnessed linker bounds; the range
 * is empty in this image, but the original indirect-call loop remains. */
void func_0010163c(void)
{
    StartupConstructor *constructor = startup_constructors_begin;
    StartupConstructor *end = startup_constructors_end;
    for (; constructor < end; ++constructor)
        (*constructor)();
}
