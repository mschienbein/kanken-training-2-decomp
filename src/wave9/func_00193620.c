#include "game/functions.h"

#include "game/matrix.h"
extern volatile unsigned int data_002438f8;
extern struct Matrix34 bss_004e3224;

const struct Matrix34 *func_00193620(void)
{
    if (!(*(const volatile unsigned char *)&data_002438f8 & 1) && func_00191c80(&data_002438f8)) {
        bss_004e3224.elements[0 * 4 + 0] = 1.0f;
        bss_004e3224.elements[0 * 4 + 1] = 0.0f;
        bss_004e3224.elements[0 * 4 + 2] = 0.0f;
        bss_004e3224.elements[0 * 4 + 3] = 0.0f;
        bss_004e3224.elements[1 * 4 + 0] = 0.0f;
        bss_004e3224.elements[1 * 4 + 1] = 1.0f;
        bss_004e3224.elements[1 * 4 + 2] = 0.0f;
        bss_004e3224.elements[1 * 4 + 3] = 0.0f;
        bss_004e3224.elements[2 * 4 + 0] = 0.0f;
        bss_004e3224.elements[2 * 4 + 1] = 0.0f;
        bss_004e3224.elements[2 * 4 + 2] = 1.0f;
        bss_004e3224.elements[2 * 4 + 3] = 0.0f;
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_002438f8);
    }
    return &bss_004e3224;
}
