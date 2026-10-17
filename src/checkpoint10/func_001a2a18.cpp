#include "game/functions.h"
#include "game/playback_screen.h"
#include "game/resource_runtime.h"
#include "game/scene_lifecycle.h"
#include "game/selection.h"
#include "game/text_writer.h"
#include "game/screen.h"

struct W24BMenuVTable;
struct W24BMenuState {
    const struct W24BMenuVTable *vtable;
    unsigned char reserved04[0x30];
    int previous_selection;
    unsigned char reserved38[0xa8];
    struct WidgetSlot **selection_slots;
    struct WidgetSlot **footer_slots;
};

/* INTEGRATOR: move to include/game/menu_state.h */
extern "C" {



void func_001c0f5c(struct WidgetSlot **slots);
void func_001c1cfc(struct WidgetSlot **slots);




extern const struct W24BMenuVTable data_00223544;
}

extern "C" void func_001a2a18(struct W24BMenuState *screen)
{
    screen->vtable = &data_00223544;
    if (func_00177124(7) == 0) {
        func_00177084();
        func_00181b58(2);
        func_001c0f5c(screen->selection_slots);
        func_001c1cfc(screen->footer_slots);
        func_00176e24(screen);
        func_00176df4();
        func_00176dc8(screen->previous_selection);
    }
    func_00174480(screen);
}
