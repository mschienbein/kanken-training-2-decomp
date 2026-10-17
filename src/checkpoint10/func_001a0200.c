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

int func_00177124(unsigned int mask);
void func_00177084(void);
void func_00181b58(unsigned short state);
void func_001c0f5c(struct WidgetSlot **slots);
void func_001c1cfc(struct WidgetSlot **slots);
void func_00176e24(struct W24BMenuState *screen);
void func_00176df4(void);
void func_00176dc8(int selection);

void func_001a0200(struct W24BMenuState *screen)
{
    if (func_00177124(7) == 0) {
        func_00177084();
        func_00181b58(2);
        func_001c0f5c(screen->selection_slots);
        func_001c1cfc(screen->footer_slots);
        func_00176e24(screen);
        func_00176df4();
        func_00176dc8(screen->previous_selection);
    }
}
