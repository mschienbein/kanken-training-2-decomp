#include "game/animation.h"
#include "game/menu_controller.h"

static void find_child(struct AnimationOwner *owner, int index, int child_index,
                        struct AnimationChild **result)
{
    *result = 0;
    if (owner->animations[index] && child_index >= 0)
        *result = owner->animations[index]->children[child_index];
}

/* [INFERENCE] Compiler-inlined helper boundaries are reconstructed; mode is
 * a real setter input, fixed to zero by the public operation below. */
static void update_text(struct AnimationOwner *owner, int index, int child_index,
                        int text, int mode)
{
    struct AnimationChild *child;
    find_child(owner, index, child_index, &child);
    if (child) {
        func_0017a968(child->object, child->table, text, mode);
        child->text_index = text;
    }
}

void func_00171d40(struct AnimationOwner *owner, int index, int child_index, int text)
{
    update_text(owner, index, child_index, text, 0);
}
