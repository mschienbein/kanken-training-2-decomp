extern void func_0019b574(unsigned parameter, unsigned *value);
extern void func_0019c9d4(unsigned queue);
extern void func_0019c920(void);

void func_00196a94(unsigned queue)
{
    unsigned previous;
    func_0019b574(0x207, &previous);
    func_0019c9d4(queue);
    func_0019c920();
    func_0019c9d4(previous);
}
