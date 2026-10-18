#include "game/character_panel.h"
#include "game/w11_control.h"
#include "game/messages.h"
#include "game/screen_operations.h"
/* [INFERENCE] The minimum setter may have been armlink-inlined and removed;
 * the inferred field operation does not establish historical helper identity. */

struct DetailController {
    unsigned char reserved00[0x50];
    struct W8DisplaySet *scene;
    unsigned char reserved54[0x18];
    struct AnimationState *animation;
    unsigned char reserved70[8];
    struct MessageLayout *text;
    struct CharacterPanel *menu;
    unsigned char reserved80[4];
    struct Screen *indicators;
};

extern void func_0017a164(struct Screen *screen, int state);
extern int func_00179ed4(struct CharacterPanel *menu, int active);
extern void func_001b5ddc(struct WidgetController *menu);
extern int func_0016dc50(struct CharacterPanel *menu, int selected);



void func_00168d14(struct DetailController *controller)
{
    int i;
    float height;
    func_0017a43c(controller->text, (const unsigned short *)L"\x3000", (const unsigned short *)L"\x3000", 0);
    for (i = 0; i < 20; ++i)
        func_0017a0e0(controller->scene, i + 8, 0);
    height = func_0017a424(controller->text);
    func_0017a258(controller->animation, height, 260.0f);
    func_0017a164(controller->indicators, func_0017a210(controller->animation));
    func_00179ed4(controller->menu, 0);
    func_001b5ddc(&controller->menu->controller);
    set_character_panel_minimum(controller->menu, 0);
    func_0016dc50(controller->menu, 0);
}
