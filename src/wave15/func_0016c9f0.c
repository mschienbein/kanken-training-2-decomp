#include "game/screen.h"
#include "game/selection.h"

#include "game/messages.h"

extern const unsigned short data_001bc29c[];
struct W15gFiveColumnScreen {
    unsigned char reserved00[0x3c];
    struct W11TextResource *dictionary;
    unsigned char reserved40[8];
    struct W11jVisualOwner *numbers;
    struct W6ListController *lists;
    void *text;
    unsigned char reserved54[0x68];
    int page;
    int last_page;
    int count;
    int selection;
    short indices[];
};

void func_0016c9f0(struct W15gFiveColumnScreen *screen)
{
    int base, i, row, index;
    func_0017a034(screen->numbers, 0, screen->page + 1);
    func_001780c4(screen->lists, 6, 0, screen->page != screen->last_page);
    func_001780c4(screen->lists, 6, 1, screen->page != 0);
    base = screen->page * 5;
    i = 0;
    for (row = 0; row < 5; ++row) {
        index = i + base;
        if (index < screen->count && index < 495)
            func_001eafa0(screen->text, i, func_00170c00(screen->dictionary, screen->indices[index]));
        else
            func_001eafa0(screen->text, i, data_001bc29c);
        ++i;
    }
    index = screen->selection % 5 + screen->page * 5;
    if (index < screen->count)
        screen->selection = index;
}
