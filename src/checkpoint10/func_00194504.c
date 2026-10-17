#include "game/functions.h"
#include "game/matrix.h"

extern volatile unsigned int data_002438fc;
extern struct Matrix44 bss_004e3254;

const struct Matrix44 *func_00194504(void)
{
    if (!(*(const volatile unsigned char *)&data_002438fc & 1) &&
        func_00191c80(&data_002438fc)) {
        bss_004e3254.elements[0] = 1.0f;
        bss_004e3254.elements[1] = 0.0f;
        bss_004e3254.elements[2] = 0.0f;
        bss_004e3254.elements[3] = 0.0f;
        bss_004e3254.elements[4] = 0.0f;
        bss_004e3254.elements[5] = 1.0f;
        bss_004e3254.elements[6] = 0.0f;
        bss_004e3254.elements[7] = 0.0f;
        bss_004e3254.elements[8] = 0.0f;
        bss_004e3254.elements[9] = 0.0f;
        bss_004e3254.elements[10] = 1.0f;
        bss_004e3254.elements[11] = 0.0f;
        bss_004e3254.elements[12] = 0.0f;
        bss_004e3254.elements[13] = 0.0f;
        bss_004e3254.elements[14] = 0.0f;
        bss_004e3254.elements[15] = 1.0f;
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_002438fc);
    }
    return &bss_004e3254;
}
