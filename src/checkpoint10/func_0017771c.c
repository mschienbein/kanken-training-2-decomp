struct QueuedValueState {
    unsigned char reserved00[0x14];
    unsigned value;
};

extern void func_00181a4c(unsigned opcode, unsigned first, unsigned second,
                          unsigned kind, unsigned flags);

void func_0017771c(struct QueuedValueState *state, unsigned value)
{
    state->value = value;
    func_00181a4c(14, value, value, 3, 0x80);
}
