#include "game/category_progress.h"



void CategoryScene::update()
{
    CategoryScene *screen = this;
    screen->prepare();
    func_001c1590(screen->frame);
    if (screen->show_category) {
        unsigned value, minutes, seconds;
        func_001dbf40(&value, &minutes, &seconds, 0);
        func_0017a034(screen->labels, 1, value);
        func_0017a034(screen->labels, 2, minutes);
        func_0017a034(screen->labels, 3, seconds);
    }
    return screen->finish();
}
