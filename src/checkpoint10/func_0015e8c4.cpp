#include "game/screen.h"

struct ChoiceItem;
class ChoiceController {
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    virtual void slot10();
    virtual void setActive(ChoiceItem *item, int active);
};
struct ChoiceBinding {
    unsigned char reserved00[0x1c];
    ChoiceController *controller;
    ChoiceItem *current;
    ChoiceItem **items;
    int previousIndex, currentIndex, count;
};
struct ChoiceDisplay {
    DisplayObject base;
    ChoiceBinding **bindings() { return reinterpret_cast<ChoiceBinding **>(this + 1); }
};

/* [INFERENCE] Selection item identities are opaque writable objects. */
extern "C" void func_0015e8c4(ChoiceDisplay *display, int binding, int selected)
{
    ChoiceBinding *choice = display->bindings()[binding];
    if (selected >= 0 && choice->count > selected) {
        if (-1 < choice->currentIndex)
            choice->controller->setActive(choice->items[choice->currentIndex], 0);
        choice->previousIndex = choice->currentIndex;
        choice->currentIndex = selected;
        choice->controller->setActive(choice->items[selected], 1);
        choice->current = choice->items[choice->currentIndex];
    }
}
