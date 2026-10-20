#ifndef GAME_MENU_CONTROLS_H
#define GAME_MENU_CONTROLS_H

#include <stdint.h>
#include "game/screen.h"
#include "game/input.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names. The scene factory, the 20 allocations of
 * 0xb8 bytes, and both list/motion consumers establish this complete family.
 * See scratch/research/type-models/menu/MODEL.md, sections 1 and 2.
 * MenuControlItem deliberately differs from the unrelated text MenuItem. */
struct MenuControlItem {
    struct DisplayObject *object;
    const struct NameTable *table;
    int16_t identifier, action;
    uint16_t flags;
    int8_t exclusive, delay;
    int8_t default_label, current_label;
    int16_t result;
    uint32_t sound, reserved18;
    struct WidgetPosition anchor, position;
    struct InputRectangle *hit_rect;
};

struct MenuGroup {
    uint32_t flags;
    int count;
    struct MenuControlItem **items;
    struct PointerSample pointer;
    struct InputRectangle viewport, limits;
    struct WidgetPosition scroll, bias, offset, velocity, delta;
    int mode, selected;
    unsigned preserve_selected;
    struct MotionState motion;
};

struct MenuControlSet {
    int bits_per_word, word_count;
    uint32_t selection_words[6];
    struct MenuGroup *groups[20];
    void *capture;
    uint8_t refreshing;
    uint8_t reserved75[3];
};

#endif
