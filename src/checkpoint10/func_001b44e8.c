/* [INFERENCE] Widget-group type; the cleanup wrapper preserves the object pointer. */
struct W24TenWidgetScreen;
extern void func_001b3c64(struct W24TenWidgetScreen *screen);

struct W24TenWidgetScreen *func_001b44e8(struct W24TenWidgetScreen *screen)
{
    func_001b3c64(screen);
    return screen;
}
