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

struct GraphicsState {
    signed char initialized;
    signed char stereo;
    unsigned char reserved02[0x1a];
    struct GraphicsContext *context;
};

struct GraphicsMutex;
extern struct GraphicsState data_002430f0;
extern struct GraphicsMutex data_00243104;
extern struct GraphicsContext *func_001861c4(void);
extern struct GraphicsMutex *func_001861d4(void);
extern int func_001847f8(void);
extern int func_00187818(void);

#endif
