#ifndef GAME_INPUT_RUNTIME_H
#define GAME_INPUT_RUNTIME_H
#include "game/service_runtime.h"
struct InputController;
struct InputLatches { unsigned char acquired[2][2]; };
/* This device-latch snapshot is not the unrelated widget/touch snapshot. */
struct InputLatchSnapshot {
    unsigned char current[2][2];
    unsigned char primary[2][2];
    unsigned char changed[2][2];
    unsigned char enabled;
    signed char selected;
};
struct InputSessionContext {
    unsigned char active;
    unsigned char reserved01[0x3b];
    int selected;
};
struct ShortInputUpdate {
    unsigned short flags, value, reserved04, mode;
    void *data;
    unsigned short parameters[4];
};
struct ExtendedInputUpdate {
    unsigned short flags, value, reserved04, mode;
    void *data[5];
    unsigned short parameters[12];
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct InputLatches bss_004b5228;
extern struct InputController bss_004b9710;
extern struct InputSessionContext bss_004b97ac;
extern void func_00137128(struct InputController *controller);
extern struct InputController *func_00185044(void);
extern struct InputLatches *func_001861e0(void);
extern int func_0015341c(struct InputLatches *state, int group, int index);
extern int func_00151cf8(struct InputLatchSnapshot *snapshot, signed char index);
extern void func_0014837c(struct InputLatchSnapshot *snapshot);
extern void func_001483c0(struct InputController *controller);
extern void func_0013712c(struct InputSessionContext *state);
extern void func_00137118(void);
#ifdef __cplusplus
}
#endif
#endif
