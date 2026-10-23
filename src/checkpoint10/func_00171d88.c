#include "game/animation.h"

int func_00171d88(const struct AnimationOwner *owner, int index, int child_index)
{
    int text = -1;
    struct Animation *const *slot = &owner->animations[index];
    if (*slot && child_index >= 0) {
        const struct AnimationChild *child = (*slot)->children[child_index];
        if (child)
            text = child->text_index;
    }
    return text;
}
