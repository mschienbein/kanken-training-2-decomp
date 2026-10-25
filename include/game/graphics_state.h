#ifndef GAME_GRAPHICS_STATE_H
#define GAME_GRAPHICS_STATE_H
#include "ctr/svc.h"
#include "game/memory_arena.h"
#include "game/synchronization.h"

struct DisplayBuffer {
    unsigned char reserved00;
    signed char active;
};

/* [INFERENCE] Relay roles; constructor0012f65c installs this subobject at+30. */
/* Keep the prepared relay value model opaque; its body is not a type cutover. */
union GraphicsRelayHeader;
struct GraphicsRelayRing {
    unsigned reserved;
    union GraphicsRelayHeader *header;
};

/* One typed GPU wire command; pointer-valued inputs are serialized as words.
 * [INFERENCE] Descriptive field names, not an alternative runtime-object view. */
struct GraphicsTransferCommand {
    struct { unsigned char opcode, reserved01, reserved02, channel; } header;
    unsigned parameters[6];
    struct { unsigned short first, second; } final;
};
struct GraphicsCommandTemplate {
    unsigned char channel;
    unsigned char reserved01[3];
    const struct GraphicsTransferCommand *command;
};
struct GraphicsCommandQueue { void *shared; };
struct GraphicsContext {
    struct StreamMutex mutex;
    unsigned char reserved0c[0x30 - 0x0c];
    struct GraphicsRelayRing relay;
    struct MappedMemoryBlock mapping;
    unsigned reserved54;
    struct GraphicsCommandQueue commands;
    struct DisplayBuffer *buffers[2];
    struct SignalState completion;
    unsigned char reserved6c[8];
    unsigned char reserved74;
    signed char status;
    unsigned char state, initialized;
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
extern void func_00137b48(struct GraphicsRelayRing *ring, int enabled);
extern CtrResult func_0012f5a0(void);
extern int func_001847f8(void);
extern int func_00187818(void);
extern struct GraphicsCommandTemplate data_00243784;
extern int func_00182bb0(struct GraphicsCommandQueue *queue,
                        const struct GraphicsTransferCommand *command);
extern void func_001376fc(void *start0, unsigned value0, void *end0,
                         unsigned width0, void *start1, unsigned value1,
                         void *end1, unsigned width1);
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
