#include "game/display_resources.h"
#include <string.h>

void func_00195900(DisplayObject *object, int index, const char *name, int mode)
{
    W16AnimatedDisplay *display = reinterpret_cast<W16AnimatedDisplay *>(object);
    strcpy(bss_0049d958, name);
    strcat(bss_0049d958, ".bclan");
    W16AnimationSlot *slot = display->animationSlot(index);
    nw::lyt::Layout *player = slot->player;
    const void *resource = slot->accessor->GetResource(0x616e696d, bss_0049d958, 0);
    if (slot->animation != 0) {
        player->UnbindAnimation(0);
        player->DeleteAnimTransform(slot->animation);
        slot->animation = 0;
    }
    slot->animation = player->CreateAnimTransform(resource, slot->accessor);
    slot->player->BindAnimation(slot->animation);
    /* ORIGINAL BUG: a nonzero index still resets playback on slot zero. */
    {
        W16AnimationSlot *first = display->animationSlot(0);
        first->frame = 0.0f;
        first->animation->SetFrame(0.0f);
    }
    W16AnimationSlot *first = display->animationSlot(0);
    if (mode != 0) {
        first->mode = 0;
        first->progress = 0.0f;
    } else {
        first->mode = 2;
    }
}
