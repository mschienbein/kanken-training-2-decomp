/* [INFERENCE] Shared lesson-scene translation unit. Address adjacency and a
 * shared scene/audio interface motivate this grouping; historical TU and
 * concrete class identities are not recovered. No context functions are
 * reproduced and no new external name bindings are proposed. */
#include "game/item_pages.h"
#include "game/text.h"
#include "game/screen.h"
#include "game/w11_control.h"
#include "game/selection.h"
#include "game/item_text.h"
#include "game/messages.h"
#include "game/command_state.h"
#include "game/display_animation.h"
#include "game/sound_controller.h"
#include "game/sound_track.h"

struct W17SceneVTable;
struct W17SceneBase {
    const W17SceneVTable *vtable;
    unsigned char reserved04[0x10];
    W10DrawingInput *drawing;
    unsigned char reserved18[0x1c];
    int index;
};

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
struct AnimationModeRecord {
    unsigned char reserved00[0x28];
    int mode;
    unsigned char reserved2c[0x28];
};
struct QueuedDisplayHandle {
    struct BoundDisplayObject *object;
    unsigned int flags;
};
struct QueuedDisplayState {
    unsigned char reserved00[0x28];
    struct QueuedDisplayHandle *handle;
};
struct IndexedRow {
    unsigned char reserved00[0x20];
    int row;
};

struct LessonGlyphs;
struct LessonLinks;
struct LessonTextCursor;

/* [INFERENCE] Field names describe their roles in the lesson display state machine. */
struct LessonDisplayState : W17SceneBase {
    struct LessonGlyphs *glyphs;
    struct LessonLinks *links;
    unsigned reserved40;
    struct W11jItemController *views;
    unsigned reserved48;
    struct W7dMenuControl *control;
    unsigned char reserved50[0x1c];
    struct AnimationState *animation;
    unsigned char reserved70[8];
    struct MessageLayout *display;
    struct QueuedDisplayState *indicator;
    struct LessonTextCursor *cursor;
    struct Screen *screen;
    unsigned char reserved88[8];
    int depth;
    int previous_depth;
    int states[8];
    int phase;
    unsigned char reservedbc[8];
    int mode;
    unsigned reservedc8;
    int item_index;
    unsigned reservedd0;
    int page_index;
    unsigned char reservedd8[0x0c];
    int control_mode;
    int audio_id;
    int audio_phase;
    int character_index;
    unsigned char reservedf4[0x10];
    int active_view;
    int lesson_view;
    unsigned char reserved10c[0xb2];
    unsigned char alternate;
    unsigned char reserved1bf;
    unsigned char showing_link;
    unsigned char reserved1c1;
    unsigned short text[128];
    unsigned short character;
    unsigned char reserved2c4[0x110];
    void *auxiliary;
};

/* [INFERENCE] Canonical partial object views overlap in these clients:
 * links/glyphs expose W11ControlState and DecoderState prefixes; links also
 * expose W11TextResource. MessageLayout's x/y and lines fields have the
 * PositionPair and IndexedRow views. Controller modes are indexed from the
 * controller base by func_00177b70. These views do not establish historical
 * class identities; integration must reconcile the shared partial types.
 * The alternate-text getter observes this scene's item/page/text fields. */

/* INTEGRATOR: move to include/game/lesson_scene.h */
extern "C" {
extern int func_0015e31c(unsigned value, unsigned parameter);
extern void func_00168a4c(struct LessonDisplayState *state, int visible, int update);
extern void func_00168dd0(struct LessonDisplayState *state, int enabled);
extern void func_00169018(struct LessonDisplayState *state, int visible);
extern void func_001690a8(struct LessonDisplayState *state, int update);
extern void func_0016f174(struct AnimationState *state, int enabled);
extern int func_00177160(struct LessonLinks *links);
extern void func_00177198(struct MessageLayout *display, const struct W11TextResource *resource, int index);
extern int func_001771fc(struct LessonLinks *links, const unsigned short *text,
                         short *indices, int count, int start);
extern void func_001772dc(struct W11ControlState *state, int value);
extern void func_001774e0(struct W11ControlState *state, int value);
extern int func_0017731c(struct LessonGlyphs *glyphs);
extern void func_00177354(struct MessageLayout *display, unsigned short character);
extern void func_001773cc(struct MessageLayout *display, struct LessonGlyphs *glyphs,
                          unsigned short character);
extern int func_0017749c(struct LessonGlyphs *glyphs);
extern void func_00177520(struct MessageLayout *display);
extern int func_00177564(struct W11jItemController *views, int index);
extern float func_001775a4(struct AnimationState *state);
extern void func_001775e0(struct MessageLayout *display);
extern void func_0017771c(struct MessageLayout *display, int audio_id);
extern void func_001779ec(struct W11jItemController *views, int index, int kind, int enabled);
extern void func_00177a84(struct W11jItemController *views, int index, int kind, int enabled);
extern void func_00177adc(struct W11jItemController *views, int index, int enabled);
extern void func_00177b28(struct W11jEnableControl *control, int enabled);
extern int func_00177b3c(const struct W11jItemController *views, int index);
extern int func_00177b70(const struct AnimationModeRecord *views, int index);
extern void func_00177b84(struct MessageLayout *display, int enabled);
extern void func_001793e4(struct QueuedDisplayState *indicator, int enabled);
extern void func_0017a164(struct Screen *screen, int state);
extern void func_0017a258(struct AnimationState *state, float end, float start);
extern float func_0017a424(const struct IndexedRow *display);
extern void func_001b3ac4(struct W11jItemController *views, int index);
extern int func_001b4920(struct LessonTextCursor *cursor);
int func_00171e30(void);
int func_00177124(int mode);
void func_00177084(void);
void func_00181b58(unsigned short screen);
void func_00168dbc(void);
void func_0016d4f8(W10DrawingInput *input);
void func_0016d310(W10DrawingInput *input);
void func_00174480(void *object);
void func_00176e24(W17SceneBase *scene);
void func_00176df4(void);
void func_00176dc8(int index);

/* A nonzero result leaves the current update owned by pending playback. */
void func_001df028(struct LessonDisplayState *state);
void func_001e0a28(struct LessonDisplayState *scene);
}

static void set_lesson_state(struct LessonDisplayState *state, int next);
static int finish_lesson_audio(struct LessonDisplayState *state);
static void hide_lesson_audio(struct LessonDisplayState *state);

void func_001df028(struct LessonDisplayState *state)
{
    /* ORIGINAL BUG: missing page/item lookups reach unchecked metadata/text getters. */
    switch (state->states[state->depth]) {
    case 0x1b:
        func_0015e31c(2, 0x100001c);
        func_00168dd0(state, 1);
        func_001b3ac4(state->views, state->lesson_view);
        func_00177adc(state->views, state->lesson_view, 0);
        if (state->alternate) {
            func_00177a84(state->views, state->lesson_view, 1, 1);
            func_001779ec(state->views, state->lesson_view, 1, 1);
        } else {
            func_001779ec(state->views, state->lesson_view, 1, 0);
        }
        state->active_view = state->lesson_view;
        func_0016f174(state->animation, 1);
        func_00177b28(reinterpret_cast<W11jEnableControl *>(state->control), 0);
        {
            int enabled = 1;
            if (state->mode == 3)
                enabled = 0;
            func_001793e4(state->indicator, enabled);
        }
        {
            int enabled = 0;
            if (state->control_mode == 1)
                func_00179068(state->control, 8, enabled);
        }
        state->showing_link = 0;
        set_lesson_state(state, 0x1c);
        break;
    case 0x1c:
        if (func_00177b3c(state->views, state->lesson_view)) {
            func_00168a4c(state, 1, 1);
            set_lesson_state(state, 0x1d);
        }
        break;
    case 0x1d:
        if (!state->showing_link) {
            if (state->phase == 0) {
                int kind, audio_id;
                struct ItemPage *page;
                struct MenuItem *item;
                state->phase = 1;
                page = func_0017a4e8(data_0023ecf4, state->page_index);
                item = func_0017a49c(page, state->item_index, 0);
                func_00177740(item, &kind, &audio_id);
                if (audio_id == -1) {
                    state->audio_phase = 0;
                } else {
                    state->audio_id = audio_id;
                    state->audio_phase = 1;
                }
                func_001690a8(state, 0);
            }
            if (state->audio_phase != 0 && state->audio_phase != 3) {
                if (state->audio_phase == 1) {
begin_audio:
                    func_0017771c(state->display, state->audio_id);
                    func_00186598();
                    ++state->audio_phase;
                    return;
                }
                if (state->audio_phase == 2) {
                    if (finish_lesson_audio(state))
                        return;
                }
            }
        }
        if (state->phase == 1) {
            state->phase = 2;
            func_00177adc(state->views, state->lesson_view, 1);
        }
        func_00177598(reinterpret_cast<PositionPair *>(state->display), func_001775a4(state->animation), 0.0f);
        if (func_00177b70(reinterpret_cast<const AnimationModeRecord *>(state->views), state->lesson_view) == 1) {
            state->showing_link = 1;
            func_00177adc(state->views, state->lesson_view, 0);
            set_lesson_state(state, 0x25);
        } else if (func_00177564(state->views, state->lesson_view)) {
            state->showing_link = 0;
            func_00168a4c(state, 0, 1);
            set_lesson_state(state, 0x1e);
        }
        break;
    case 0x1e:
        if (state->audio_phase != 0 && state->audio_phase != 3) {
            if (state->audio_phase == 1) {
                goto begin_audio;
            }
            if (state->audio_phase == 2) {
                if (finish_lesson_audio(state))
                    return;
            }
        }
        {
            int result = func_00177b70(reinterpret_cast<const AnimationModeRecord *>(state->views), state->lesson_view);
            if (result == 0) {
                func_001b3ac4(state->views, 0xd);
                func_00177adc(state->views, 0xd, 0);
                state->active_view = 0xd;
                func_00177b84(state->display, 0);
                func_00177520(state->display);
                set_lesson_state(state, 0x1f);
            } else if (result == 2) {
                state->active_view = -1;
                func_00177b28(reinterpret_cast<W11jEnableControl *>(state->control), 1);
                func_0016f174(state->animation, 0);
                {
                    int enabled = 1;
                    if (state->mode == 3)
                        enabled = 0;
                    func_001793e4(state->indicator, enabled);
                }
                {
                    int enabled = 1;
                    if (state->control_mode == 1)
                        func_00179068(state->control, 8, enabled);
                }
                {
                    int *depth = &state->depth;
                    int previous = *depth;
                    *depth = 4;
                    state->previous_depth = previous;
                    state->states[*depth] = 0x15;
                    state->phase = 0;
                }
            }
        }
        break;
    case 0x1f:
        if (func_00177b3c(state->views, 0xd)) {
            func_00168a4c(state, 1, 0);
            hide_lesson_audio(state);
            func_001774e0(reinterpret_cast<W11ControlState *>(state->glyphs), 1);
            set_lesson_state(state, 0x20);
        }
        break;
    case 0x20:
        if (!func_001dcebc(reinterpret_cast<const DecoderState *>(state->glyphs))) {
            struct ItemPage *page;
            const unsigned short *text;
            int audio_id;
            state->character_index = 0;
            page = func_0017a4e8(data_0023ecf4, state->page_index);
            func_0017a49c(page, state->item_index, 0);
            text = func_00168e80(reinterpret_cast<W12cAlternateTextState *>(state));
            state->character = text[state->character_index];
            audio_id = func_0017749c(state->glyphs);
            if (audio_id == -1) {
                state->character = 0;
            } else {
                func_0017771c(state->display, audio_id);
                func_00186598();
            }
            set_lesson_state(state, 0x21);
        }
        break;
    case 0x21:
        if (!func_0019efd0()) {
            if (state->phase == 0) {
                state->phase = 1;
                func_00177354(state->display, state->character);
            } else {
                func_001773cc(state->display, state->glyphs, state->character);
                func_0017a258(state->animation, func_0017a424(reinterpret_cast<const IndexedRow *>(state->display)), 260.0f);
                func_0017a164(state->screen, func_0017a210(state->animation));
                set_lesson_state(state, 0x22);
            }
        }
        break;
    case 0x22:
        if (state->phase == 0) {
            state->phase = 1;
            func_00177adc(state->views, 0xd, 1);
        }
        if (func_00177564(state->views, 0xd)) {
            func_0017731c(state->glyphs);
            func_00168a4c(state, 0, 1);
            func_00177b84(state->display, 0);
            func_00177520(state->display);
            set_lesson_state(state, 0x24);
        } else {
            struct ItemPage *page;
            const unsigned short *text;
            int length, index;
            page = func_0017a4e8(data_0023ecf4, state->page_index);
            func_0017a49c(page, state->item_index, 0);
            text = func_00168e80(reinterpret_cast<W12cAlternateTextState *>(state));
            length = func_0017adc4(text);
            index = func_001b4920(state->cursor);
            if (index >= 0 && index < length && state->character_index != index) {
                int audio_id;
                struct ItemPageSet *set;
                state->character_index = index;
                func_0015e31c(1, 0x1000015);
                set = data_0023ecf4;
                page = func_0017a4e8(set, state->page_index);
                func_0017a49c(page, state->item_index, 0);
                text = func_00168e80(reinterpret_cast<W12cAlternateTextState *>(state));
                state->character = text[state->character_index];
                audio_id = func_0017749c(state->glyphs);
                if (audio_id == -1) {
                    state->character = 0;
                } else {
                    func_0017771c(state->display, audio_id);
                    func_00186598();
                }
                func_00177adc(state->views, 0xd, 0);
                set_lesson_state(state, 0x21);
            } else {
                func_00177598(reinterpret_cast<PositionPair *>(state->display), func_001775a4(state->animation), 0.0f);
            }
        }
        break;
    case 0x24:
        if (func_00177b70(reinterpret_cast<const AnimationModeRecord *>(state->views), 0xd) != -1) {
            func_001b3ac4(state->views, state->lesson_view);
            func_00177adc(state->views, state->lesson_view, 0);
            state->active_view = state->lesson_view;
            func_00177b84(state->display, 0);
            func_00177520(state->display);
            set_lesson_state(state, 0x1c);
        }
        break;
    case 0x25:
        func_001772dc(reinterpret_cast<W11ControlState *>(state->links), 1);
        set_lesson_state(state, 0x26);
        break;
    case 0x26:
        if (!func_001dcae0(reinterpret_cast<const DecoderState *>(state->links))) {
            struct ItemPage *page = func_0017a4e8(data_0023ecf4, state->page_index);
            struct MenuItem *item = func_0017a49c(page, state->item_index, 0);
            const unsigned short *text = func_00179090(item);
            if (text) {
                short indices[8];
                int found = func_001771fc(state->links, text, indices, 8, 0);
                func_00177198(state->display, reinterpret_cast<const W11TextResource *>(state->links), found ? indices[0] : -1);
                func_0017a258(state->animation, func_0017a424(reinterpret_cast<const IndexedRow *>(state->display)), 260.0f);
                func_0017a164(state->screen, func_0017a210(state->animation));
            }
            func_00177160(state->links);
            func_00177adc(state->views, state->lesson_view, 1);
            set_lesson_state(state, 0x1d);
        }
        break;
    }
}

void func_001e0a28(LessonDisplayState *scene)
{
    if (func_00171e30() == 0 || func_00177124(7) != 0)
        return;
    func_00177084();
    func_00181b58(6);
    func_00168dbc();
    hide_lesson_audio(scene);
    /* [INFERENCE] callee identity: inlined by armlink */
    func_00174480(scene->auxiliary);
    if (scene->drawing != 0) {
        func_0016d4f8(scene->drawing);
        func_0016d310(scene->drawing);
    }
    switch (scene->mode) {
    case 0: scene->index = 7; break;
    case 1: scene->index = 9; break;
    case 2: scene->index = 8; break;
    case 3: scene->index = 10; break;
    }
    func_00176e24(scene);
    func_00176df4();
    func_00176dc8(scene->index);
}

static void set_lesson_state(struct LessonDisplayState *state, int next)
{
    state->states[state->depth] = next;
    state->phase = 0;
}

static int finish_lesson_audio(struct LessonDisplayState *state)
{
    if (func_0019efd0())
        return 1;
    func_001775e0(state->display);
    func_00169018(state, 1);
    state->audio_phase++;
    return 0;
}

static void hide_lesson_audio(struct LessonDisplayState *state)
{
    if (state->audio_phase != 0) {
        func_00169018(state, 0);
        state->audio_phase = 0;
    }
}
