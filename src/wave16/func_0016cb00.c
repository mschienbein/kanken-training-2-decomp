#include "game/messages.h"
#include "game/w11_control.h"
#include "game/screen.h"

struct W11TextResource;
struct MessageLayout;

struct SelectionController {
    unsigned char reserved00[0x3c];
    const struct W11TextResource *archive;
    unsigned char reserved40[0x2c];
    struct AnimationState *animation;
    unsigned char reserved70[8];
    struct MessageLayout *text;
    unsigned char reserved7c[0x48];
    int count;
    int selected;
    short entries[514];
    struct Screen *indicators;
};

extern void func_00177198(struct MessageLayout *layout, const struct W11TextResource *archive, int index);

extern float func_0017a424(const struct MessageLayout *layout);
extern void func_0017a258(struct AnimationState *state, float content, float viewport);
extern int func_0017a210(const struct AnimationState *state);
extern void func_0016d554(struct Screen *screen, int state);

void func_0016cb00(struct SelectionController *controller)
{
    if (!controller->count)
        return;
    if (controller->selected < controller->count) {
        float height;
        func_00177198(controller->text, controller->archive, controller->entries[controller->selected]);
        height = func_0017a424(controller->text);
        func_0017a258(controller->animation, height, 260.0f);
        func_0016d554(controller->indicators, func_0017a210(controller->animation));
    }
}
