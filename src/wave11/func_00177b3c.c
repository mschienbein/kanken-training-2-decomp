#include "game/screen.h"

struct W11jListRecord {
    int state;
    unsigned reserved04;
    float duration;
    int elapsed;
    int current;
    unsigned char reserved14[0x0c];
    const struct NameTable *table;
    unsigned char reserved24[0x10];
    struct DisplayObject *object;
    unsigned char reserved38[0x1c];
};

struct W11jItemController {
    unsigned char reserved00[0x18];
    struct W11jListRecord records[23];
};

int func_00177b3c(const struct W11jItemController *controller, int index)
{
    const struct W11jListRecord *record = &controller->records[index];
    if (record->object != 0 && record->state == 2)
        return 1;
    return 0;
}
