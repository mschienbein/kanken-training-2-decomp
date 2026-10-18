struct W22WidgetMask {
    int bit_count, word_count;
    unsigned mask[1];
    W22WidgetMask(int count)
    {
        bit_count = count;
        word_count = (count + 31) / 32;
        for (int i = 0; i < word_count; ++i)
            mask[i] = 0;
    }
};
struct W22LargeHandle;
struct TouchFrame;
struct W22LargeResourceGroup : W22WidgetMask {
    W22LargeHandle *handles[27];
    unsigned reserved78[3];
    TouchFrame *inputs[2];
    W22LargeResourceGroup();
    ~W22LargeResourceGroup();
};
/* INTEGRATOR: move to include/game/resource_groups.h */
extern "C" {
W22LargeResourceGroup *func_001b50bc(void);
}

W22LargeResourceGroup *func_001b50bc(void)
{
    /* [INFERENCE] callee identity: static destructor registration and guard release were inlined by armlink. */
    static W22LargeResourceGroup group;
    return &group;
}
