#ifndef GAME_CHOICE_PANE_CONTROLLERS_H
#define GAME_CHOICE_PANE_CONTROLLERS_H
#include "game/input.h"
#include "game/screen.h"

/* [INFERENCE] Shared partial controller/entry names and roles. */
struct ChoiceEntry {
    int state;
    unsigned short flags;
    unsigned short reserved06;
    float remaining;
    int selected;
    int result;
    unsigned sounds[3];
    const NameTable *tables[5];
    DisplayObject *objects[5];
    InputRectangle *regions[3];
};

/* [INFERENCE] Data-state name, not the polymorphic ChoiceController used by
 * ChoiceBinding. The original 001b3434 context uses word zero as a column
 * divisor; 001b376c consumes its entry array beginning at +0x18. */
struct ChoiceGridState {
    int columns;
    unsigned char reserved04[0x0c];
    unsigned pending[2];
    ChoiceEntry entries[23];
    const NameTable *const *tables;
};

extern "C" void func_001b376c(ChoiceGridState *controller, int index);
#endif
