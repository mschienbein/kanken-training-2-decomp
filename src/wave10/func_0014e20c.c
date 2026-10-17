struct HwrChannelState {
    unsigned char reserved[0x6c];
    unsigned short flags;
};

extern void func_0014e218(struct HwrChannelState *state);

void func_0014e20c(struct HwrChannelState *state)
{
    state->flags = 0xffff;
    func_0014e218(state);
}
