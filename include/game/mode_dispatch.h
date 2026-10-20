#ifndef GAME_MODE_DISPATCH_H
#define GAME_MODE_DISPATCH_H



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct ModeDispatchObject;

struct ModeDispatchObject {
    unsigned char reserved00[0x90];
    int mode;
};

extern "C" {
void func_001ec890(ModeDispatchObject *object);
void func_001ed698(ModeDispatchObject *object);
void func_001ec194(ModeDispatchObject *object);
}
#endif

#endif
