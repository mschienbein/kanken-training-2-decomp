#ifndef GAME_INPUT_RUNTIME_H
#define GAME_INPUT_RUNTIME_H
#include "game/service_runtime.h"
struct InputController;
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
    unsigned char reserved01[3];
    float first_gain, volume;
    float channels[2];
    AuxCallback callbacks[2];
    void *contexts[2];
    unsigned char reserved24[0x0c];
    signed char fallback[2];
    unsigned char reserved32[2];
    struct StreamMutex mutex;
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
/* Two 20-byte primary descriptors precede the variable-length channel vector
 * at +0x28. No bound for the complete controller allocation is assumed. */
struct InputController {
    struct ShortInputUpdate primary[2];
#ifndef __cplusplus
    struct ExtendedInputUpdate channels[];
#endif
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct InputLatchSnapshot bss_004b5228;
extern struct InputController bss_004b9710;
extern struct InputSessionContext bss_004b97ac;
extern void func_00137128(struct InputController *controller);
extern struct InputController *func_00185044(void);
extern struct InputLatchSnapshot *func_001861e0(void);
extern int func_0015341c(struct InputLatchSnapshot *state, int group, int index);
extern int func_00151cf8(struct InputLatchSnapshot *snapshot, signed char index);
extern int func_0014ded8(struct InputLatchSnapshot *snapshot, signed char index,
                          const struct ShortInputUpdate *update);
extern int func_0014df74(struct InputLatchSnapshot *snapshot, signed char index,
                          const struct ExtendedInputUpdate *update);
extern int func_0014df0c(struct InputController *controller, int index,
                          const struct ShortInputUpdate *update);
extern int func_0014dfa8(struct InputController *controller, int index,
                          const struct ExtendedInputUpdate *update);
extern int func_00180fe0(struct ServiceCoordinator *coordinator, int index,
                          const struct ShortInputUpdate *update);
extern int func_00180e74(struct ServiceCoordinator *coordinator, int index,
                          const struct ExtendedInputUpdate *update);
extern void func_00180d9c(struct ServiceCoordinator *coordinator, int index, float value);
extern void func_0014837c(struct InputLatchSnapshot *snapshot);
extern void func_001483c0(struct InputController *controller);
extern void func_00146188(struct InputController *controller);
extern void func_0013712c(struct InputSessionContext *state);
extern void func_00137118(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void func_0014b244(struct InputSessionContext *context, signed char index,
                          AuxCallback callback, void *argument);
extern void func_00152428(struct InputSessionContext *context, int index, float value);
extern void func_001487cc(struct InputSessionContext *context, float value);
extern void func_0014cbf4(struct InputSessionContext *context, float value);
extern void func_0017e858(struct InputSessionContext *context, unsigned index, unsigned char enabled);
#ifdef __cplusplus
}
#endif

#endif
