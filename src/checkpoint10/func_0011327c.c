extern int func_00121018(unsigned *count, unsigned archive, unsigned mode, unsigned id,
                         unsigned first, unsigned second, unsigned reserved);

int func_0011327c(unsigned *count, unsigned archive, unsigned id,
                  unsigned first, unsigned second, unsigned reserved)
{
    return func_00121018(count, archive, 0, id, first, second, reserved);
}
