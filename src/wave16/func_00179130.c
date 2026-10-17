/* Position slots retain a caller-provided Y coordinate when absent. */
struct PositionSlot {
    float reserved00;
    float x;
    float y;
};

struct PositionScreen {
    unsigned char reserved00[0x1300];
    struct PositionSlot *slots[];
};

extern void func_00166eb0(void *screen, int index, float x, float y);

void func_00179130(struct PositionScreen *screen, int index, float x, float y)
{
    struct PositionSlot *slot = screen->slots[index];
    if (slot != 0)
        y = slot->y;
    func_00166eb0(screen, index, x, y);
}
