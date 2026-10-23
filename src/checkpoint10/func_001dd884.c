#include "game/item_text.h"
#include "game/text.h"
#include "game/selection.h"


struct W19PracticeScreen {
    unsigned char reserved00[0x14];
    struct W10DrawingInput *effects;
    unsigned char reserved18[0x34];
    struct W7dMenuControl *control;
    unsigned char reserved50[0x7c];
    int item_index;
    unsigned reservedd0;
    int page_index;
    unsigned reservedd8;
    int mode;
    unsigned reservede0;
    int active;
    unsigned char reservede8[0x20];
    int side;
    unsigned char reserved10c[0x0c];
    int selected;
    unsigned char reserved11c[0xa2];
    unsigned char has_link;
};




void func_001dd884(struct W19PracticeScreen *screen)
{
    struct ItemPageSet *set = data_0023ecf4;
    struct ItemPage *page = func_0017a4e8(set, screen->page_index);
    /* ORIGINAL BUG: page and item lookup results are used without null checks. */
    struct MenuItem *item = func_0017a49c(page, screen->item_index, 0);
    int kind = item->text.kind;
    screen->active = 0;
    screen->mode = 0;
    screen->selected = -1;
    switch (kind) {
    case 0: case 1: case 3: case 25:
        func_0016dc94(screen->effects, 1);
        goto finish;
    case 4:
        func_0016dc94(screen->effects, 2);
        goto finish;
    case 8: case 9: case 10: case 11: case 12: case 13: case 15:
    case 20: case 21: case 22: case 23:
        func_0016dc94(screen->effects, 0);
        goto finish;
    case 14:
        func_0016dc94(screen->effects, 6);
        goto finish;
    case 32:
        screen->active = 2;
        break;
    default:
        screen->active = 1;
        if (func_0017adc4(func_001791e0(&item->text, 0)) == 1)
            screen->mode = 1;
        else
            screen->mode = 2;
        break;
    }
finish:
    screen->side = func_00179148() ? 11 : 12;
    if (func_00179090(item))
        screen->has_link = 1;
    else
        screen->has_link = 0;
    if (screen->active == 1)
        func_00179068(screen->control, 8, 1);
    else
        func_00179068(screen->control, 8, 0);
}
