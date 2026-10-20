#include "game/selection.h"

static void draw_number(struct W11jVisualOwner *controller, int index, int value)
{
    struct W11jVisualNode *group = controller->nodes[index];
    if (group) {
        const struct W11jNumberFormat *format = &controller->formats[index];
        switch (format->kind) {
        case 0: case 6:
            func_001ea314(controller, index, format, value);
            break;
        case 1:
            func_001ea1cc(controller, index, format, value);
            break;
        case 2: case 3: case 4: case 5:
            func_001ea0cc(controller, index, format, value);
            break;
        }
    }
}

void func_0017a034(struct W11jVisualOwner *controller, int index, int value)
{
    func_00166f20(controller, index, 0);
    draw_number(controller, index, value);
    func_00166f20(controller, index, 1);
}
