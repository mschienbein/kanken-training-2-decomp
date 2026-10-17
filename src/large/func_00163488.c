#include "game/functions.h"
#include "game/selection.h"

struct ActionPanel;
struct GradeOptionScreen {
    unsigned char reserved00[0x48];
    struct ActionPanel *panel;
    struct W6ListController *list;
    unsigned char reserved50[8];
    struct W6Selector *selector;
};

extern unsigned func_0017418c(void);




extern void func_001741f8(struct ActionPanel *panel, int index, float x, float y);
extern void func_0017a034(struct ActionPanel *panel, int index, int value);

void func_00163488(struct GradeOptionScreen *screen, int from_list)
{
    unsigned flags = func_0017418c();
    int selected;
    if (!from_list) {
        selected = func_001795bc();
    } else {
        for (selected = 0; selected < 12; ++selected) {
            if (func_00174160(screen->list, 2, selected) == 1)
                break;
        }
        if (selected == 12)
            return;
    }
    func_00166f1c(screen->panel, 3, 0);
    if (func_00174444() == 1) {
        if (selected == -1) {
            func_00174210(screen->selector, 1);
            return;
        }
        switch (selected) {
        case 1: case 3:
            func_00174210(screen->selector, 11);
            func_001741f8(screen->panel, 3, -79.0f, 96.0f);
            break;
        case 11:
            func_00174210(screen->selector, 12);
            func_001741f8(screen->panel, 3, -90.0f, 96.0f);
            break;
        default:
            func_00174210(screen->selector, 10);
            func_001741f8(screen->panel, 3, -86.0f, 96.0f);
            break;
        }
        func_0017a034(screen->panel, 3, selected);
        return;
    } else {
        if (!func_00174430()) {
            if (!from_list) {
                func_00174210(screen->selector, 7);
                return;
            }
            flags = 0;
        } else if (selected == -1) {
            if (func_00174470() == -1)
                func_00174210(screen->selector, 0);
            else
                func_00174210(screen->selector, 1);
            return;
        }
        if (flags & (1U << selected)) {
            switch (selected) {
            case 1: case 3:
                func_00174210(screen->selector, 14);
                func_001741f8(screen->panel, 3, -18.0f, 96.0f);
                break;
            case 11:
                func_00174210(screen->selector, 15);
                func_001741f8(screen->panel, 3, -22.0f, 96.0f);
                break;
            default:
                func_00174210(screen->selector, 13);
                func_001741f8(screen->panel, 3, -26.0f, 96.0f);
                break;
            }
        } else {
            switch (selected) {
            case 1: case 3:
                func_00174210(screen->selector, 11);
                func_001741f8(screen->panel, 3, -83.0f, 96.0f);
                break;
            case 11:
                func_00174210(screen->selector, 12);
                func_001741f8(screen->panel, 3, -84.0f, 96.0f);
                break;
            default:
                func_00174210(screen->selector, 10);
                func_001741f8(screen->panel, 3, -86.0f, 96.0f);
                break;
            }
        }
    }
    func_0017a034(screen->panel, 3, selected);
}
