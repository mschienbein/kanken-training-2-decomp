#include "game/functions.h"

struct SliderState {
    unsigned char reserved00[0x10];
    void *primary;
    unsigned char reserved14[0x40];
    int enabled;
    float minimum;
    float maximum;
    float value;
};

extern volatile unsigned int data_0023ecc0;
extern struct SliderState bss_0024424c;
extern struct SliderState *func_001b43bc(struct SliderState *slider);
extern struct SliderState *func_001b44e8(struct SliderState *slider);

struct SliderState *func_00167284(void)
{
    if (!(*(const volatile unsigned char *)&data_0023ecc0 & 1) &&
        func_00191c80(&data_0023ecc0)) {
        struct SliderState *slider = func_001b43bc(&bss_0024424c);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00216140(slider, (ObjectDestructor)func_001b44e8, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_0023ecc0);
    }
    return &bss_0024424c;
}
