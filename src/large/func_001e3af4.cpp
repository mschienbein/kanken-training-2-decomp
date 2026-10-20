#include "game/input.h"
#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/notification.h"
#include "game/screen.h"

/* [INFERENCE] Scene base: only the nine-slot vtable shape is evidenced (slot 8 is
 * called at the end of every update, slot 5 is the transition descriptor target). */

struct L042IntroScene : CallbackTarget {
    unsigned char reserved04[0x30];
    int leaving;
    unsigned char reserved38[0x1c];
    Screen *buttons;
    unsigned char reserved58[8];
    Screen *selection;
    unsigned char reserved64[0x2c];
    int phase;
    float remaining;
    int sound;
    int page;
    int question;
    MenuOptionGroup *first_options;
    MenuOptionGroup *second_options;
};

struct L042IntroStep {
    short buttons;
    short first_option;
    short selection;
    short second_option;
    float seconds;
};

extern "C" {
extern L042IntroStep data_0023f02c[];
extern const MemberCallback data_002184c8;
extern int func_00177124(int channel);
extern int func_0015e31c(unsigned value, unsigned parameter);
extern void func_00168118(MenuOptionGroup *options, int index);
extern void func_00167f7c(MenuOptionGroup *options, int index);
extern void func_00181b68(int channel, int close, int immediate);
extern void func_0015d6bc(int question);
extern void func_00169b58(void);
extern void func_0015e260(void);
extern int func_00169974(void);
extern int func_001682b4(void);
extern void func_0015d770(void);
extern int func_001b6d44(MenuOptionGroup *options);
extern int func_001b7f50(MenuOptionGroup *options);
}

static __forceinline void schedule_transition(CallbackTarget *scene)
{
    /* [INFERENCE] This descriptor selects the scene's virtual transition slot. */
    func_00177b10(scene, data_002184c8);
}

static __forceinline unsigned pressed_keys(void)
{
    return bss_004972d8.buttons;
}

extern "C" void func_001e3af4(L042IntroScene *scene)
{
    int next_phase;
    switch (scene->phase) {
    case 1:
        func_0015d6bc(scene->question);
        func_00169b58();
        func_0015e260();
        next_phase = 2;
        goto set_phase;
    case 2:
        if (!func_00169974())
            goto update;
        {
            int result = func_001682b4();
            if (result == 1)
                goto enter_phase6;
            if (result == 2 || result == 3)
                goto enter_phase3;
            if (++scene->question >= 5)
                goto enter_phase0;
            next_phase = 1;
            goto set_phase;
        }
enter_phase3:
        next_phase = 3;
        goto set_phase;
enter_phase0:
        next_phase = 0;
        goto set_phase;
    case 3:
        func_0017a6f4(scene->buttons, data_0023f02c[4].buttons);
        func_00168118(scene->first_options, data_0023f02c[4].first_option);
        func_001795cc(scene->selection, data_0023f02c[4].selection, -1);
        func_00167f7c(scene->second_options, data_0023f02c[4].second_option);
        func_00181b68(7, 0, 1);
        next_phase = 4;
        goto set_phase;
    case 4:
        if (func_00177124(7))
            goto update;
        func_0015d770();
        func_00169b58();
        next_phase = 5;
        goto set_phase;
    case 5:
        if (!func_00169974())
            goto update;
        {
            int result = func_001682b4();
            if (result == 1 || result == 2)
                goto enter_phase8;
            if (result == 3)
                goto enter_phase3;
            goto enter_phase6;
        }
enter_phase8:
        next_phase = 8;
        goto set_phase;
    case 6:
        func_0015d6e0(scene->question);
        func_00169b58();
        next_phase = 7;
        goto set_phase;
    case 7:
        if (!func_00169974())
            goto update;
        {
            int result = func_001682b4();
            if (result == 1 || result == 2)
                goto enter_phase8;
            if (result == 3)
                goto enter_phase3;
            if (++scene->question >= 5)
                goto close_question;
        }
enter_phase6:
        next_phase = 6;
        goto set_phase;
close_question:
        func_00181b68(7, 1, 1);
        next_phase = 0;
        goto set_phase;
    case 8:
        func_00181b68(7, 1, 1);
        next_phase = 9;
        goto set_phase;
    case 0:
        if (func_00177124(7))
            goto update;
        {
            L042IntroStep *step = data_0023f02c + scene->page;
            if (scene->page >= 3)
                scene->sound = func_0015e31c(~0u, 0x1000014);
            func_0017a6f4(scene->buttons, step->buttons);
            func_00168118(scene->first_options, step->first_option);
            func_001795cc(scene->selection, step->selection, -1);
            func_00167f7c(scene->second_options, step->second_option);
            scene->remaining = step->seconds * 1000.0f;
        }
        func_00181b68(7, 0, 1);
        next_phase = 11;
set_phase:
        scene->phase = next_phase;
        goto update;
    case 11:
        if (func_00177124(7))
            goto update;
        scene->remaining -= func_00191134();
        if (scene->remaining <= 0.0f || (pressed_keys() & 0x204000u)) {
            func_00181b68(7, 1, 1);
            next_phase = 12;
            goto set_phase;
        }
        goto update;
    case 12:
        if (!func_00177124(7))
            goto advance_page;
        goto update;
    default:
        goto update;
    }
update:
    func_001b6d44(scene->first_options);
    func_001b7f50(scene->second_options);
    scene->callback8();
    return;
advance_page:
    if (++scene->page >= 4) {
        scene->leaving = 1;
        schedule_transition(scene);
        return;
    }
    goto enter_phase0;
}
