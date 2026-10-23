struct Screen;
extern void func_00170e58(struct Screen *screen, int index, int enabled);

void func_001795cc(struct Screen *screen, int first, int second)
{
    func_00170e58(screen, first, 1);
    if (second != -1)
        func_00170e58(screen, second, 1);
}
