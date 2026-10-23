struct Screen;
extern void func_0017a9b4(struct Screen *screen, int value);
extern void func_0017a71c(struct Screen *screen, int index, int enabled);

void func_00176ae8(struct Screen *screen, int first, int second)
{
    func_0017a9b4(screen, 0);
    func_0017a71c(screen, first, 1);
    func_0017a71c(screen, second, 1);
}
