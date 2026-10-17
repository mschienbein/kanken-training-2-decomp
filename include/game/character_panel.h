#ifndef GAME_CHARACTER_PANEL_H
#define GAME_CHARACTER_PANEL_H

#include "game/screen.h"

/* Generic slot operations consume the WidgetController prefix, not the
 * different extent of a menu Screen. The selection fields fix 28 slot positions. */
struct CharacterPanel {
    struct WidgetController controller;
    struct WidgetSlot *slots[28];
    int character_count;                    /* 0x7c */
    int minimum;                            /* 0x80 */
    unsigned char reserved84[0x0c];
    int selected;                           /* 0x90 */
};

#ifdef __cplusplus
extern "C" {
#endif
extern void func_00179724(struct CharacterPanel *panel, int primary, int secondary);
#ifdef __cplusplus
}
#endif

#endif
