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
struct W22Handle;
struct TouchFrame;
struct W22ResourceGroup : W22WidgetMask {
    W22Handle *handles[25];
    TouchFrame *inputs[12];
    W22ResourceGroup();
    ~W22ResourceGroup();
};
/* INTEGRATOR: move to include/game/resource_groups.h */
extern "C" {
W22ResourceGroup *func_001ada1c(void);
}

W22ResourceGroup *func_001ada1c(void)
{
    /* [INFERENCE] callee identity: static destructor registration and guard release were inlined by armlink. */
    static W22ResourceGroup group;
    return &group;
}
