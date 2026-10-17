struct ScreenAsset;
struct AssetSlot {
    struct ScreenAsset *asset;
};
struct AnimationTarget;
extern struct AnimationTarget bss_00290f8c;
extern void func_00183208(struct ScreenAsset *asset);
extern void func_0017a938(struct AnimationTarget *target, int index, unsigned flags);

void func_001c3618(struct AssetSlot **slots)
{
    int i;
    for (i = 0; i < 5; ++i) {
        struct AssetSlot *slot = slots[i];
        if (!slot)
            continue;
        if (slot->asset)
            func_00183208(slot->asset);
        slot = slots[i];
        if (slot)
            slot->asset = 0;
    }
    func_0017a938(&bss_00290f8c, -1, 0);
}
