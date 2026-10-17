struct PlaybackItem;

extern const struct PlaybackItem *bss_00290e98[];
extern void func_0017a7e4(const struct PlaybackItem *const *items, int count,
                          unsigned mode, int volume);

void func_001c20d0(void)
{
    func_0017a7e4(bss_00290e98, -1, 3, 128);
}
