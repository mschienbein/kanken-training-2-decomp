extern void func_0019b574(unsigned parameter, unsigned *value);
extern void func_0019c9d4(unsigned queue);
extern void func_0011dd34(void);

unsigned func_0010b56c(unsigned queue)
{
    unsigned previous;
    func_0019b574(0x207, &previous);
    func_0019c9d4(queue);
    func_0011dd34();
    return previous;
}
