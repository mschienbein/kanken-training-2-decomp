#ifndef GAME_APPLET_H
#define GAME_APPLET_H

struct AppletServiceNames;
struct ServiceManagerState;
struct SharedAppletState;

#include <stdbool.h>
#include "ctr/svc.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct SharedAppletState {
    bool initialized;
    unsigned char event;
    unsigned char reserved02[2];
    unsigned handle;
    void *memory;
};

struct AppletServiceNames {
    const char *primary;
    const char *fallback;
};

struct ServiceManagerState {
    unsigned char initialized;
    unsigned char reserved01[3];
    int references;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct SharedAppletState data_00243034;
extern void func_0010d454(bool notify);
extern CtrResult func_00115ad4(unsigned state);
extern unsigned char bss_004b35c4[0x1000];
extern struct AppletServiceNames data_00243000;
extern void func_0012fd88(const char *name);
extern unsigned func_0019ecf8(void);
extern CtrResult func_00115b0c(void);
extern int func_0018f068(unsigned handle, unsigned char *value);
extern void func_00192afc(int result, const void *pc);
extern void func_0019ec88(void);
extern CtrResult func_00138274(void *buffer, unsigned size);
extern int func_001382c0(unsigned value);
extern CtrResult func_0012ff20(unsigned applet_id, unsigned char mode);
extern struct ServiceManagerState data_0024313c;
extern unsigned func_001221e8(unsigned *handle, const char *name,
                              int length, unsigned flags);
extern void func_0019242c(void);
extern int func_0012fee8(unsigned value);
extern unsigned func_00192b44(unsigned *handle, const char *name,
                      int length, unsigned flags);
extern void func_001155f8(void *memory);
extern void func_00115608(void);
extern unsigned char *func_0011561c(void);
/* The receive event is forwarded as a full word and stored as one byte. */
extern void func_00115628(int event);
extern void func_0011587c(unsigned handle);
extern void func_00122ae0(void);
extern int func_0012fda0(unsigned int flags);
extern void func_0012fe8c(void);
extern void func_001923d4(unsigned char mode);
extern void func_00195e64(bool notify);
extern void func_0019ec64(void);
#ifdef __cplusplus
}
#endif

#endif
