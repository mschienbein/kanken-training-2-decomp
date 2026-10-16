struct LeafState { char reserved[0x1c]; int count; };
void func_0010e664(struct LeafState *state) { state->count = 0; }

struct Allocation { void *pointer; int count; };
extern struct Allocation data_00242220;
extern void func_0019dc20(void *allocation);
void func_00103798(void)
{
    if (data_00242220.pointer) {
        func_0019dc20(data_00242220.pointer);
        data_00242220.count = 0;
    }
}

struct Position {
    char reserved0[0x1c];
    int value;
    char reserved1[8];
    float x;
    char reserved2[4];
    float y;
    int flags;
    char reserved3[4];
};
extern struct Position bss_0049c118[];
void func_001091d4(int index, int value, int x, int y)
{
    struct Position *position = &bss_0049c118[index];
    position->value = value;
    position->flags = 0;
    position->x = (float)x;
    position->y = (float)y;
}

struct Ratio {
    char reserved0[0x4c];
    float denominator;
    char reserved1[0xc];
    float numerator;
};
float func_001103d4(struct Ratio *ratio)
{
    return ratio->numerator / ratio->denominator;
}
