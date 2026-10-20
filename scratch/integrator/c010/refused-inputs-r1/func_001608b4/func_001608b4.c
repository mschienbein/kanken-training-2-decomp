#include "game/item_pages.h"
#include "game/text.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/w11_control.h"
#include "game/command_state.h"
#include "game/messages.h"

struct W11jEnableControl {
    unsigned char reserved00[0x74];
    unsigned char disabled;
};

struct W11jListRecord {
    int state;
    unsigned reserved04;
    float duration;
    int elapsed;
    int current;
    unsigned char reserved14[0x0c];
    const struct NameTable *table;
    unsigned char reserved24[0x10];
    struct DisplayObject *object;
    unsigned char reserved38[0x1c];
};

struct W11jItemController {
    unsigned char reserved00[0x18];
    struct W11jListRecord records[23];
};

struct QueuedDisplayHandle {
    struct BoundDisplayObject *object;
    unsigned int flags;
};

struct QueuedDisplayState {
    unsigned char reserved00[0x28];
    struct QueuedDisplayHandle *handle;
};

struct W11jFlagBinding {
    struct DisplayObject *object;
    unsigned flags;
};

struct W11jBindingOwner {
    unsigned char reserved00[0x30];
    struct W11jFlagBinding *binding;
};

struct AnimationModeRecord {
    unsigned char reserved00[0x28];
    int mode;
    unsigned char reserved2c[0x28];
};

struct IndexedRow {
    unsigned char reserved00[0x20];
    int row;
};

struct SelectionAnimation;
struct CharacterResource;
struct LinkedResource;
struct StrokeDisplay;
struct TextPanel;
struct CursorControl;

struct SelectionStateIndices {
    int current;
    int previous;
};

/* [INFERENCE] Descriptive field names; the fixed layout follows the accesses. */
struct SelectionPlaybackState {
    unsigned char reserved00[0x38];
    struct CharacterResource *character_resource;
    struct LinkedResource *linked_resource;
    unsigned reserved40;
    struct SelectionAnimation *animation;
    unsigned reserved48;
    struct W7dMenuControl *control;
    unsigned char reserved50[0x1c];
    struct AnimationState *transition;
    unsigned char reserved70[8];
    struct StrokeDisplay *stroke;
    struct TextPanel *text_panel;
    struct CursorControl *cursor;
    struct Screen *status_screen;
    unsigned char reserved88[8];
    struct SelectionStateIndices indices;
    int states[3];
    int step;
    unsigned char reserveda8[8];
    int selection_kind;
    int item_index;
    unsigned reservedb8;
    int page_index;
    int mode;
    unsigned reservedc4;
    int active;
    int side;
    int context_id;
    int context_state;
    int text_index;
    unsigned char reserveddc[0xa4];
    unsigned char has_link;
    unsigned char reserved181;
    unsigned char restart;
    unsigned char reserved183[0x101];
    unsigned short character;
};

extern int func_0015e31c(unsigned value, unsigned parameter);
extern void func_001799b4(struct SelectionPlaybackState *state, int enabled);
extern void func_00179a48(struct SelectionPlaybackState *state, int enabled);
extern void func_001b3ac4(struct SelectionAnimation *animation, int index);
extern void func_00177adc(struct SelectionAnimation *animation, int index, int enabled);
extern void func_001779ec(struct SelectionAnimation *animation, int index, int first, int second);
extern void func_00177a84(struct SelectionAnimation *animation, int index, int first, int second);
extern void func_0016f174(struct AnimationState *state, int enabled);
extern void func_00177b28(struct W11jEnableControl *control, int enabled);
extern void func_001793e4(struct QueuedDisplayState *state, int active);
extern void func_0017939c(struct W11jBindingOwner *owner, int enabled);
extern int func_00177b3c(const struct W11jItemController *controller, int index);
extern void func_0017775c(struct SelectionPlaybackState *state, int first, int second);
extern struct MenuItem *func_0017a49c(struct ItemPage *page, int index, int *changed);
extern void func_00177740(const struct MenuItem *item, int *first, int *second);
extern void func_00179ad8(struct SelectionPlaybackState *state, int mode);
extern void func_001775e0(struct StrokeDisplay *display);
extern float func_001775a4(const struct AnimationState *state);
extern void func_00177598(struct StrokeDisplay *display, float first, float second);
extern int func_00177b70(const struct AnimationModeRecord *records, int index);
extern int func_00177564(const struct SelectionAnimation *animation, int index);
extern void func_0017771c(struct StrokeDisplay *display, int resource);
extern void func_00177b84(struct StrokeDisplay *display, int enabled);
extern void func_00177520(struct StrokeDisplay *display);
extern void func_001774e0(struct W11ControlState *state, int value);
extern int func_001dcebc(const struct CharacterResource *resource);
extern const unsigned short *func_001790a8(struct SelectionPlaybackState *state);
/* The code unit is forwarded unchanged to func_00169878. */
extern int func_0017749c(const struct CharacterResource *resource, unsigned short character);
extern void func_001773cc(struct StrokeDisplay *display, struct CharacterResource *resource, unsigned short character);
extern float func_0017a424(const struct IndexedRow *state);
extern void func_0017a258(struct AnimationState *state, float first, float second);
extern void func_0017a164(struct Screen *screen, int state);
extern void func_00177354(struct StrokeDisplay *display, unsigned short character);
extern void func_0017731c(struct CharacterResource *resource);
extern int func_001b4920(struct CursorControl *cursor);
extern void func_001772dc(struct W11ControlState *state, int value);
extern int func_001dcae0(const struct LinkedResource *resource);
extern int func_001771fc(struct LinkedResource *resource, const unsigned short *text, short *indices, int capacity, int mode);
extern void func_00177198(struct MessageLayout *display, const struct W11TextResource *resource, int index);
extern void func_00177160(struct LinkedResource *resource);

static __inline void set_state(struct SelectionPlaybackState *state, int value)
{
    state->states[state->indices.current] = value;
    state->step = 0;
}

static __inline void set_text_enabled(struct SelectionPlaybackState *state, int enabled)
{
    /* [INFERENCE] These partial receiver layouts are views of the same panel. */
    func_001793e4((struct QueuedDisplayState *)state->text_panel, enabled);
    func_0017939c((struct W11jBindingOwner *)state->text_panel, enabled);
}

static __inline void set_optional_control_enabled(struct SelectionPlaybackState *state, int enabled)
{
    if (state->active == 1)
        func_00179068(state->control, 2, enabled);
}

static __inline void restore_state(struct SelectionPlaybackState *state)
{
    struct SelectionStateIndices saved = state->indices;
    state->indices.current = saved.previous;
    state->indices.previous = saved.current;
    state->step = 0;
}

/* The item lookup retains its page-state side effect when the result is unused. */
static __inline struct MenuItem *current_item(struct SelectionPlaybackState *state)
{
    struct ItemPageSet *set = data_0023ecf4;
    int index = state->page_index;
    struct ItemPage *page = func_0017a4e8(set, index);
    return func_0017a49c(page, state->item_index, 0);
}

static __inline void stop_context(struct SelectionPlaybackState *state)
{
    if (state->context_state) {
        func_00179a48(state, 0);
        state->context_state = 0;
    }
}

static __inline int complete_context(struct SelectionPlaybackState *state)
{
    if (func_0019efd0())
        return 0;
    func_001775e0(state->stroke);
    func_00179a48(state, 1);
    state->context_state++;
    return 1;
}

void func_001608b4(struct SelectionPlaybackState *state)
{
    int value;
    unsigned short character;
    /* ORIGINAL BUG: failed page/item lookups are used without null checks. */
    switch (state->states[state->indices.current]) {
    case 3:
        func_0015e31c(2, 0x100001c);
        func_001799b4(state, 1);
        func_001b3ac4(state->animation, state->side);
        func_00177adc(state->animation, state->side, 0);
        if (state->has_link) {
            func_00177a84(state->animation, state->side, 1, 1);
            func_001779ec(state->animation, state->side, 1, 1);
        } else {
            func_001779ec(state->animation, state->side, 1, 0);
        }
        func_0016f174(state->transition, 1);
        func_00177b28((struct W11jEnableControl *)state->control, 0);
        set_text_enabled(state, 0);
        set_optional_control_enabled(state, 0);
        state->restart = 0;
        set_state(state, 4);
        break;
    case 4:
        if (func_00177b3c((const struct W11jItemController *)state->animation, state->side)) {
            func_0017775c(state, 1, 1);
            set_state(state, 5);
        }
        break;
    case 5:
        if (!state->restart) {
            if (!state->step) {
                int unused;
                int resource;
                struct MenuItem *item;
                state->step = 1;
                item = current_item(state);
                func_00177740(item, &unused, &resource);
                if (resource == -1) {
                    state->context_state = 0;
                } else {
                    state->context_id = resource;
                    state->context_state = 1;
                }
                func_00179ad8(state, 0);
            }
            value = state->context_state;
            if (value != 0 && value != 3) {
                if (value == 1) {
                    /* Both playback phases share the asynchronous resource-start path. */
start_context:
                    func_0017771c(state->stroke, state->context_id);
                    func_00186598();
                    state->context_state++;
                    return;
                }
                if (value == 2) {
                    if (!complete_context(state))
                        return;
                }
            }
        }
        if (state->step == 1) {
            state->step = 2;
            func_00177adc(state->animation, state->side, 1);
        }
        func_00177598(state->stroke, func_001775a4(state->transition), 0.0f);
        if (func_00177b70((const struct AnimationModeRecord *)state->animation, state->side) == 1) {
            state->restart = 1;
            func_00177adc(state->animation, state->side, 0);
            set_state(state, 13);
            return;
        }
        if (func_00177564(state->animation, state->side)) {
            state->restart = 0;
            func_0017775c(state, 0, 1);
            set_state(state, 6);
        }
        break;
    case 6:
        value = state->context_state;
        if (value != 0 && value != 3) {
            if (value == 1) {
                goto start_context;
            }
            if (value == 2) {
                if (!complete_context(state))
                    return;
            }
        }
        value = func_00177b70((const struct AnimationModeRecord *)state->animation, state->side);
        if (value == 0) {
            func_001b3ac4(state->animation, 13);
            func_00177adc(state->animation, 13, 0);
            func_00177b84(state->stroke, 0);
            func_00177520(state->stroke);
            set_state(state, 7);
        } else if (value == 2) {
            func_00177b28((struct W11jEnableControl *)state->control, 1);
            func_0016f174(state->transition, 0);
            value = 1;
            if (state->selection_kind == 3)
                value = 0;
            set_text_enabled(state, value);
            set_optional_control_enabled(state, 1);
            restore_state(state);
        }
        break;
    case 7:
        if (func_00177b3c((const struct W11jItemController *)state->animation, 13)) {
            func_0017775c(state, 1, 0);
            stop_context(state);
            func_001774e0((struct W11ControlState *)state->character_resource, 1);
            set_state(state, 8);
        }
        break;
    case 8:
        if (!func_001dcebc(state->character_resource)) {
            const unsigned short *text;
            state->text_index = 0;
            current_item(state);
            text = func_001790a8(state);
            character = text[state->text_index];
            state->character = character;
            value = func_0017749c(state->character_resource, character);
            if (value == -1) {
                state->character = 0;
            } else {
                func_0017771c(state->stroke, value);
                func_00186598();
            }
            set_state(state, 9);
        }
        break;
    case 9:
        if (!func_0019efd0()) {
            if (!state->step) {
                state->step = 1;
                func_00177354(state->stroke, state->character);
            } else {
                func_001773cc(state->stroke, state->character_resource, state->character);
                func_0017a258(state->transition, func_0017a424((const struct IndexedRow *)state->stroke), 260.0f);
                func_0017a164(state->status_screen, func_0017a210(state->transition));
                set_state(state, 10);
            }
        }
        break;
    case 10:
        if (!state->step) {
            state->step = 1;
            func_00177adc(state->animation, 13, 1);
        }
        if (func_00177564(state->animation, 13)) {
            func_0017731c(state->character_resource);
            func_0017775c(state, 0, 1);
            func_00177b84(state->stroke, 0);
            func_00177520(state->stroke);
            set_state(state, 12);
        } else {
            int length;
            int cursor;
            current_item(state);
            length = func_0017adc4(func_001790a8(state));
            cursor = func_001b4920(state->cursor);
            if (cursor >= 0 && cursor < length && cursor != state->text_index) {
                const unsigned short *text;
                state->text_index = cursor;
                func_0015e31c(1, 0x1000015);
                current_item(state);
                text = func_001790a8(state);
                character = text[state->text_index];
                state->character = character;
                value = func_0017749c(state->character_resource, character);
                if (value == -1) {
                    state->character = 0;
                } else {
                    func_0017771c(state->stroke, value);
                    func_00186598();
                }
                func_00177adc(state->animation, 13, 0);
                set_state(state, 9);
            } else {
                func_00177598(state->stroke, func_001775a4(state->transition), 0.0f);
            }
        }
        break;
    case 12:
        if (func_00177b70((const struct AnimationModeRecord *)state->animation, 13) != -1) {
            func_001b3ac4(state->animation, state->side);
            func_00177adc(state->animation, state->side, 0);
            func_00177b84(state->stroke, 0);
            func_00177520(state->stroke);
            set_state(state, 4);
        }
        break;
    case 13:
        func_001772dc((struct W11ControlState *)state->linked_resource, 1);
        set_state(state, 14);
        break;
    case 14:
        if (!func_001dcae0(state->linked_resource)) {
            struct MenuItem *item = current_item(state);
            const unsigned short *text = func_00179090(item);
            if (text) {
                short indices[8];
                int count = func_001771fc(state->linked_resource, text, indices, 8, 0);
                func_00177198((struct MessageLayout *)state->stroke, (const struct W11TextResource *)state->linked_resource, count ? indices[0] : -1);
                func_0017a258(state->transition, func_0017a424((const struct IndexedRow *)state->stroke), 260.0f);
                func_0017a164(state->status_screen, func_0017a210(state->transition));
            }
            func_00177160(state->linked_resource);
            func_00177adc(state->animation, state->side, 1);
            set_state(state, 5);
        }
        break;
    }
}
