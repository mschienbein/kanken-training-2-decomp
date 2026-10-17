struct PositionSlot {
    void *object;
    float x;
    float y;
};
struct PositionScreen {
    unsigned char reserved00[0x1300];
    struct PositionSlot *slots[];
};
extern void func_00166eb0(void *screen, int index, float x, float y);

void func_001eaf84(struct PositionScreen *screen, int index, float y)
{
    struct PositionSlot *slot = screen->slots[index];
    float x = y;
    if (slot != 0)
        x = slot->x;
    func_00166eb0(screen, index, x, y);
}
