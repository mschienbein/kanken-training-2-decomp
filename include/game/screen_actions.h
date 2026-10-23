#ifndef GAME_SCREEN_ACTIONS_H
#define GAME_SCREEN_ACTIONS_H
#include "game/screen.h"

/* [INFERENCE] Partial action-owner and sentinel-terminated action records.
 * WidgetPosition retains the independent s0/s1 value-argument contract. */
struct ScreenAction { int index; struct WidgetPosition position; };
struct ScreenActionOwner {
    unsigned char reserved00[0xee];
    unsigned char flags;
    unsigned char reservedef[0x210 - 0xef];
    struct DisplayHandle **handles;
};
#ifdef __cplusplus
extern "C" {
#endif
void func_0016c358(struct DisplayHandle *const *handles, int index, int active,
                   struct WidgetPosition position);
void func_001c3988(struct DisplayHandle *const *handles, int index, int value);
void func_0016c0f8(struct ScreenActionOwner *owner, const struct ScreenAction *action);
#ifdef __cplusplus
}
#endif
#endif
