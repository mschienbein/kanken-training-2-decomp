#ifndef GAME_GRAPHICS_STATE_H
#define GAME_GRAPHICS_STATE_H

struct DisplayBuffer {
    unsigned char reserved00;
    signed char active;
};

struct GraphicsContext {
    unsigned char reserved00[0x5c];
    struct DisplayBuffer *buffers[];        /* 0x5c; count not known */
};

struct GraphicsMutex { unsigned handle; };
struct GraphicsState {
    signed char initialized;
    signed char stereo;
    unsigned char reserved02[0x0e];
    unsigned handle;
    struct GraphicsMutex mutex;
    unsigned reserved18;
    struct GraphicsContext *context;
};
#ifdef __cplusplus
extern "C" {
#endif
extern int func_0012f570(const void *buffer, unsigned size);
extern int func_0018f6c4(const void *buffer, unsigned size);
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

#endif
