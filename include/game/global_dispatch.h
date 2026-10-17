#ifndef GAME_GLOBAL_DISPATCH_H
#define GAME_GLOBAL_DISPATCH_H

struct GlobalDispatchVTable;
struct GlobalDispatchObject;



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */


struct GlobalDispatchVTable {
    unsigned char reserved00[8];
    void (*dispatch)(GlobalDispatchObject *object);
};
struct GlobalDispatchObject {
    const GlobalDispatchVTable *vtable;

    void dispatch()
    {
        vtable->dispatch(this);
    }
};

extern "C" {

extern GlobalDispatchObject bss_0028fd24;
void func_001db930(void);
extern GlobalDispatchObject bss_0028dd7c;
void func_001db944(void);
}
#endif

#endif
