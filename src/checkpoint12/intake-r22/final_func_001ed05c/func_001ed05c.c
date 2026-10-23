#include "game/category_progress.h"
#include "game/notification.h"

void func_001ed05c(struct CategoryScreen *screen)
{
    int i;
    int first_slot;
    int second_slot;
    int third_slot;
    int first, second, third;
    unsigned char extra;
    func_00166f1c(screen->numbers, 5, 0);
    func_00166f1c(screen->numbers, 6, 0);
    switch (screen->selected_category) {
    case 1:
    case 3:
        func_001741f8(screen->numbers, 5, -61.0f, 96.0f);
        func_001741f8(screen->numbers, 6, 37.0f, -97.0f);
        func_00174210(screen->selector, 120);
        break;
    case 11:
        func_001741f8(screen->numbers, 5, -65.0f, 96.0f);
        func_001741f8(screen->numbers, 6, 32.0f, -97.0f);
        func_00174210(screen->selector, 121);
        break;
    default:
        func_001741f8(screen->numbers, 5, -69.0f, 96.0f);
        func_001741f8(screen->numbers, 6, 28.0f, -97.0f);
        func_00174210(screen->selector, 119);
    }
    func_0017a034(screen->numbers, 5, screen->selected_category);
    func_0017a034(screen->numbers, 6, screen->selected_category);
    if (screen->selected_category >= 11)
        func_001780c4(screen->choices, 3, 1, 0);
    else
        func_001780c4(screen->choices, 3, 1, 1);
    if (screen->selected_category <= 0)
        func_001780c4(screen->choices, 3, 2, 0);
    else
        func_001780c4(screen->choices, 3, 2, 1);
    func_001c1190(screen->frame, 1, screen->selected_category);
    first_slot = 30;
    second_slot = 40;
    third_slot = 50;
    for (i = 0; i < 10; ++i) {
        if (func_001dbacc(screen->selected_category, i, &first, &second, &third, &extra)) {
            func_0017a034(screen->numbers, first_slot, first);
            func_0017a034(screen->numbers, second_slot, second);
            func_0017a034(screen->numbers, third_slot, third);
        } else {
            func_00166f1c(screen->numbers, first_slot, 0);
            func_00166f1c(screen->numbers, second_slot, 0);
            func_00166f1c(screen->numbers, third_slot, 0);
        }
        ++third_slot;
        ++second_slot;
        ++first_slot;
    }
}
