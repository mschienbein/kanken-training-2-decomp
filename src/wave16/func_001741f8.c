struct W16PositionedDisplay {
    unsigned char reserved00[0x80];
    float position[2];
};

struct W16WidgetSet {
    unsigned char reserved00[0x7ec];
    struct W16PositionedDisplay *objects[];
};

void func_001741f8(struct W16WidgetSet *set, int index, float x, float y)
{
    struct W16PositionedDisplay *object = set->objects[index];
    if (object != 0) {
        object->position[0] = x;
        object->position[1] = y;
    }
}
