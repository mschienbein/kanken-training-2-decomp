struct HwrChannelState {
    unsigned char reserved[0x6c];
    unsigned short flags;
};
struct HwrUpdateChannel {
    unsigned char reserved00[0x68];
    struct HwrChannelState *state;
};
struct HwrUpdateOwner {
    unsigned char reserved00[0x18dc];
    struct HwrUpdateChannel *channels[24];
};
extern void func_0014e218(struct HwrChannelState *state);

void func_00154078(struct HwrUpdateOwner *owner)
{
    int i;
    for (i = 0; i < 24; ++i)
        func_0014e218(owner->channels[i]->state);
}
