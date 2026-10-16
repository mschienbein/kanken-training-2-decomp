struct Slot { void *handle; };
extern void *bss_00290f60[];
extern "C" void *func_0017a818(void *resource, int mode, int flags, int option,
                              float x, float y, float scale);
extern "C" void func_0016bf90(Slot **slots, int index, int preset)
{
    Slot *slot = slots[index];
    if (slot && !slot->handle) {
        float scale = 0.0f;
        switch (preset) {
        case 0:
        case 1: scale = 0.5f; break;
        case 2: scale = 0.6f; break;
        case 3:
        case 4: scale = 2.0f; break;
        }
        slot->handle = func_0017a818(bss_00290f60[preset], 4, 0, 0,
                                    0.0f, 0.0f, scale);
    }
}
