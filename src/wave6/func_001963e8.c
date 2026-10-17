struct ResourceEvent;

extern struct ResourceEvent *func_00145b5c(int screen, int index);
extern void func_001963f8(struct ResourceEvent *event);

void func_001963e8(int screen, int index)
{
    func_001963f8(func_00145b5c(screen, index));
}
