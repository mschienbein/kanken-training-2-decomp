/* [INFERENCE] Role names: bss_0028d95c is a fade/tint state whose float at
 * +0x1c is selected by flag bits 0-1 and whose float at +0x40 is selected by
 * flag bit 2 (0.0f otherwise). The reserved padding is not recovered. */
struct FadeState {
    unsigned char reserved00[0x1c];
    float first;
    unsigned char reserved20[0x20];
    float second;
};

extern struct FadeState bss_0028d95c;

/* The original reloads the state's address in each arm; the pointer-returning
 * accessors (inlined by the compiler) reproduce that, a hoisted base does not. */
static __inline const float *FirstValue(void)
{
    return &bss_0028d95c.first;
}

static __inline const float *SecondValue(void)
{
    return &bss_0028d95c.second;
}

float func_0015e508(unsigned flags)
{
    if (flags & 3)
        return *FirstValue();
    if (flags & 4)
        return *SecondValue();
    return 0.0f;
}
