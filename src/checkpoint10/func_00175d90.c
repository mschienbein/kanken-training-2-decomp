#include "game/screen.h"

struct NumericPanelRecord {
    int kind;
    int value;
    unsigned reserved08;
    int layer;
    float x;
    float y;
    float depth;
};
struct NumericPanelHandle {
    unsigned reserved00;
    struct DisplayObject *objects[10];
    unsigned char reserved2c[0x54];
    float x;
    float y;
    float horizontal_offset;
    float vertical_offset;
};
struct NumericPanelSet {
    unsigned reserved00[3];
    struct NumericPanelRecord records[72];
    struct NumericPanelHandle *handles[];
};
extern void func_001eaac0(struct NumericPanelSet *set, int index);

void func_00175d90(struct NumericPanelSet *set)
{
    int i;
    for (i = 0; i < 72; ++i) {
        if (set->records[i].kind == -1)
            return;
        func_001eaac0(set, i);
    }
}
