#ifndef GAME_PLAYBACK_SCREEN_H
#define GAME_PLAYBACK_SCREEN_H

#include "game/command_state.h"
#include "game/deferred_callback.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/runtime.h"
#include "game/render_parameter.h"
#include "game/item_entries.h"
#include "game/input.h"
#include <string.h>

#ifdef __cplusplus
/* [INFERENCE] Native identities and roles; layouts follow matching consumers. */
struct ScreenInitializationState;
struct ScreenInitializationState;
struct L043PlaybackEntry;

struct ScreenInitializationState : CallbackTarget {
    unsigned char reserved04[0x48];
    struct W6ListController *actions;
    struct W8DisplaySet *screen;
    struct Screen *selection;
    unsigned char reserved58[8];
    struct Screen *alternate;
    unsigned char reserved64[0x24];
    struct WidgetActionState *action;
    unsigned reserved8c;
    int mode;
    float elapsed;
    int selected;
    int count;
    struct MenuOptionGroup *first_options;
    struct MenuOptionGroup *second_options;
    struct MotionState animation;
};

struct L043PlaybackEntry {
    short selection, first, alternate, second;
    float seconds;
};

extern "C" {
extern void func_0019eb48(void);
extern struct MenuOptionGroup *func_001b6bf4(void);
extern struct MenuOptionGroup *func_001b7e00(void);
extern void func_001b6d2c(struct MenuOptionGroup *options);
extern void func_001b7f38(struct MenuOptionGroup *options);
extern struct ParameterValue *func_001757d0(struct ParameterValue *parameter);
extern struct ParameterValue *func_00175648(struct ParameterValue *parameter);
extern void func_00175654(struct Screen *list, struct ParameterValue *const *values);
extern void func_001754d0(struct Screen *list, struct ParameterValue *const *values);
extern struct W11PositionCommand *func_00174d9c(struct W11PositionCommand *command,
    int resource, int kind, unsigned option, float x, float y, float z);
extern struct ArrayOwner *func_00174cc8(struct ArrayOwner *owner, int index, int count, ...);
extern struct ArrayOwner *func_00174cb0(struct ArrayOwner *owner);
extern struct ArrayOwner *func_001748b8(struct ArrayOwner *owner);
extern void func_001748ec(struct W6ListController *list, struct ArrayOwner *const *values);
extern struct VectorParameter *func_00174f04(struct VectorParameter *parameter,
    const wchar_t *extra, int first, int second, float x, float y, float z);
extern struct VectorParameter *func_00174ef8(struct VectorParameter *parameter);
extern void func_00174df0(struct W8DisplaySet *screen, struct VectorParameter *const *values);
extern void func_00186598(void);
extern const MemberCallback data_002188d0;
extern void func_001e81fc(struct ScreenInitializationState *state);
extern const struct L043PlaybackEntry data_0023f068[];
extern void func_00168118(struct MenuOptionGroup *options, int selection);
extern const MemberCallback data_002188e0;
extern void func_00167f7c(struct MenuOptionGroup *options, int selection);
extern int func_0017abf8(int channel, int sound);
extern void func_00181b68(int channel, int mode, int enabled);
extern int func_00177124(int channel);
extern int func_0015e31c(unsigned value, unsigned parameter);
extern int func_0015e2b4(unsigned value);
extern void func_001b6d44(struct MenuOptionGroup *options);
extern void func_001b7f50(struct MenuOptionGroup *options);
extern float func_0016e874(const struct MotionState *state);
extern void func_001e84ec(struct ScreenInitializationState *state);
}
#endif

#endif
