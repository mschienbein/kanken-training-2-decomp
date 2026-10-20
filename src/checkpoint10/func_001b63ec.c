/* [INFERENCE] Widget-group type; the cleanup wrapper preserves the object pointer. */
struct W24ExtendedWidgetScreen;
extern void func_001b5114(struct W24ExtendedWidgetScreen *screen);

struct W24ExtendedWidgetScreen *func_001b63ec(struct W24ExtendedWidgetScreen *screen)
{
    func_001b5114(screen);
    return screen;
}
