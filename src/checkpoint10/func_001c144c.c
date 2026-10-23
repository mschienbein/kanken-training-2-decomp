#include "game/random.h"
#include "game/screen.h"

#include "game/category_progress.h"

extern struct ResourceEvent *func_0016eea4(
    unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha,
    int layer, int active, int kind,
    float width, float height, float x, float y, float z);

static void create_bar(struct L044TrainingFrame *panel, int index)
{
    struct WidgetSlot *slot = panel->slots[index];
    if (slot != 0 && slot->object == 0) {
        float x = -111.0f;
        float y = 106.0f - (float)(index - 5) * 15.0f;
        float z = 1.0f;
        float width = (float)func_0018ffac(224);
        float height = 11.0f;
        struct ResourceEvent *object = func_0016eea4(
            81, 212, 204, 255, 4, 0, 2, width, height, x, y, z);
        func_00180358(object != 0 ? &object->base : 0, -1, 0);
        slot->object = object;
    }
}

void func_001c144c(struct L044TrainingFrame *panel)
{
    int i;
    func_0016bf90(panel, 0, 0);
    func_0016bf90(panel, 1, 1);
    func_0016bf90(panel, 2, 1);
    func_0016bf90(panel, 3, 1);
    func_0016bf90(panel, 4, 2);
    for (i = 0; i < 12; ++i) {
        create_bar(panel, i + 5);
    }
}
