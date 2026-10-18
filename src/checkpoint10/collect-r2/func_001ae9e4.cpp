#include <stddef.h>
#include "game/screen.h"
#include "game/input.h"
#include "game/runtime.h"

/* [INFERENCE] Native mask base shared by the selection/graph screen. The
 * constructor in the address neighbour initializes a single 32-bit word. */
struct Tu606Mask {
    int bits_per_word, word_count;
    unsigned words[1];
};
/* The neighbouring native constructor allocates one pointer per slot; the
 * larger generic WidgetSlot is not this allocation's complete object type. */
struct Tu606DisplaySlot { DisplayObject *object; };
struct Tu606SelectionPanel : Tu606Mask {
    Tu606DisplaySlot *slots[25];
    InputSnapshot *inputs[12];
    int selected, hovered, previous_hovered, value;
    unsigned resource_index;
    int texture;
    int enabled;
};

/* INTEGRATOR: move to include/game/selection_panel.h. The common native owner
 * replaces the previous W24FilterScreen/W19SelectionPanel partial views. */
extern "C" {
extern void func_00183208(DisplayObject *object);
Tu606SelectionPanel *func_001ae9e4(Tu606SelectionPanel *panel);
}

static void release_display(Tu606DisplaySlot *slot)
{
    if (slot != 0 && slot->object != 0)
        func_00183208(slot->object);
}

static void reset_display(Tu606DisplaySlot *slot)
{
    if (slot != 0)
        slot->object = 0;
}

static void destroy_input_snapshot(InputSnapshot *input)
{
    /* [INFERENCE] callee identity: empty rectangle and snapshot destruction
     * inlined by armlink; the retained no-op identity remains unresolved. */
    void *rectangle = func_0018f7f0(&input->rectangle);
    input = reinterpret_cast<InputSnapshot *>(
        static_cast<char *>(rectangle) - offsetof(InputSnapshot, rectangle));
    func_001852f4(func_0018f7f0(input));
}

Tu606SelectionPanel *func_001ae9e4(Tu606SelectionPanel *panel)
{
    for (int i = 0; i < 25; ++i) {
        Tu606DisplaySlot *slot = panel->slots[i];
        if (slot != 0) {
            release_display(slot);
            reset_display(panel->slots[i]);
            func_001852f4(slot);
            panel->slots[i] = 0;
        }
    }
    for (int i = 0; i < 12; ++i) {
        if (panel->inputs[i] != 0)
            destroy_input_snapshot(panel->inputs[i]);
        panel->inputs[i] = 0;
    }
    return panel;
}
