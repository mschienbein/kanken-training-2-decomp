struct HwrChannelState {
    unsigned char reserved[0x6c];
    unsigned short flags;
};

struct HwrChannel {
    unsigned char reserved[0x68];
    struct HwrChannelState *state;
};

struct HwrChannelOwner {
    unsigned char reserved[0x18dc];
    struct HwrChannel *channels[24];
};

extern void func_0014e20c(struct HwrChannelState *state);

void func_0014603c(struct HwrChannelOwner *owner)
{
    int i;
    for (i = 0; i < 24; ++i)
        func_0014e20c(owner->channels[i]->state);
}
