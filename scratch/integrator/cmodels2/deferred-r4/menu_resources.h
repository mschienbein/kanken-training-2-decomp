#ifndef GAME_MENU_RESOURCES_H
#define GAME_MENU_RESOURCES_H

#include "game/input.h"
#include "game/runtime.h"

struct ResourceEvent;
struct NameTable;
#ifdef __cplusplus
namespace MenuResources {

/* [INFERENCE] Native point/rectangle lifetime names and historical outlining are
 * unrecovered. These candidate-only destructors have no original address binding. */
struct Point {
    float x, y;
    ~Point();
};
struct Rectangle {
    float x, y, width, height;
    ~Rectangle();
    static void operator delete(void *allocation) { func_001852f4(allocation); }
};
struct TouchHistory {
    unsigned char active, released, reserved02[2];
    InputPoint position;
    unsigned char previous_active, previous_released, reserved0e[2];
    InputPoint previous_position;
    unsigned char tracking, reserved19[3];
    InputPoint start_position;
};
/* The identifier at +0xa is distinct from AnimationChild::label_index at +8.
 * This is a separate receiver view, not a common complete-class assertion. */
struct Item {
    ResourceEvent *object;
    const NameTable *table;
    unsigned short reserved08;
    short identifier;
    unsigned short flags;
    signed char behavior, countdown, initial, current;
    short state;
    unsigned parameter;
    float value;
    Point position, size;
    Rectangle *hit_bounds;
    ~Item();
    static void operator delete(void *allocation) { func_001852f4(allocation); }
};
struct Panel {
    unsigned flags;
    int count;
    Item **items;
    TouchHistory input;
    InputRectangle bounds, area;
    float values[8];
    unsigned reserved70[2];
    int selected, hovered;
    unsigned state;
    MotionState motion;
};
struct State {
    int bit_count, word_count;
    unsigned mask[6];
    Panel *panels[20];
    const NameTable **labels;
};

}
extern "C" {
void func_00183208(struct ResourceEvent *object);
void func_0019e08c(void *allocation);
void func_0016f024(struct MotionState *state);
void func_001e9ea4(MenuResources::State *menu, int index);
void func_001e958c(MenuResources::State *menu);
}
#endif

#endif
