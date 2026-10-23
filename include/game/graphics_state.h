#ifndef GAME_GRAPHICS_STATE_H
#define GAME_GRAPHICS_STATE_H
#include "ctr/svc.h"
#include "game/synchronization.h"

struct DisplayBuffer {
    unsigned char reserved00;
    signed char active;
};

struct GraphicsContext {
    struct StreamMutex mutex;
    unsigned char reserved0c[0x50];
    /* Constructor 0012f65c installs exactly two shared-memory buffer pointers;
     * teardown 0012f894 clears both, independently of the completion event. */
    struct DisplayBuffer *buffers[2];
    struct SignalState completion;
    unsigned char reserved6c[8];
    unsigned char reserved74;
    signed char status;
    unsigned char state;
};

struct GraphicsMutex { CtrHandle handle; };
struct GraphicsState {
    signed char initialized;
    signed char stereo;
    signed char flag_02;
    unsigned char reserved03[9];
    unsigned word_0c;
    unsigned handle;
    struct GraphicsMutex mutex;
    unsigned reserved18;
    struct GraphicsContext *context;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_00122a6c(void);
extern int func_0012f570(const void *buffer, unsigned size);
extern int func_0018f6c4(const void *buffer, unsigned size);
extern CtrHandle data_0021c608;
extern struct GraphicsState data_002430f0;
extern struct GraphicsMutex data_00243104;
extern struct GraphicsContext *func_001861c4(void);
extern struct GraphicsMutex *func_001861d4(void);
extern unsigned func_0012fab0(const struct GraphicsMutex *mutex);
extern int func_001847f8(void);
extern int func_00187818(void);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
extern void func_001228c0(void);
extern void func_001228bc(void);
extern int func_001229bc(void);
extern int func_001229b8(void);
extern void func_0012f84c(struct GraphicsContext *context);
#ifdef __cplusplus
}
#endif

#endif
