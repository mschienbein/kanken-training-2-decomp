#include "game/screen.h"

struct W11ControlScreen {
    unsigned char reserved00[0x0c];
    struct WidgetSlot *slots[10];
    int layout;
    int active;
    int selection;
};

struct W11TouchState {
    unsigned char reserved00[0x18];
    unsigned flags;
    unsigned char reserved1c[0x1c];
    float x;
    float y;
};

extern struct W11TouchState bss_004972d8;
extern void func_0016ff08(struct W11ControlScreen *screen, int index, float x, float y);


void func_001b0188(struct W11ControlScreen *screen)
{
    int x;
    int column;
    int y;
    if (!(bss_004972d8.flags & 0x200000))
        return;
    x = (int)bss_004972d8.x;
    y = (int)bss_004972d8.y;
    column = 0;
    while (column < 5) {
        int right = (column + 1) * 41 + 58;
        if (x >= 58 + 41 * column && x < right && y >= 30 && y < 194)
            break;
        ++column;
    }
    if (column == 5)
        return;
    screen->selection = 4 - column;
    func_0016ff08(screen, 6, 82.0f - (4 - column) * 41.0f, 8.0f);
}
