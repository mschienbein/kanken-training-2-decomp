#ifndef GAME_COMMAND_BUFFER_H
#define GAME_COMMAND_BUFFER_H

struct MatrixObject;
/* [INFERENCE] Cursor-oriented renderer protocol; not the deferred c009
 * W19CommandBuffer interface. No W19 callable completion is introduced. */
struct RenderCache { unsigned values[11]; };
struct W15iRenderer;
struct W15iRendererVtable {
    unsigned char reserved00[0x10];
    unsigned char *(*begin)(struct W15iRenderer *, unsigned char *, unsigned flags);
    unsigned reserved14;
    unsigned char *(*end)(struct W15iRenderer *, unsigned char *, unsigned flags);
};
struct W15iRenderer {
    const struct W15iRendererVtable *vtable;
};

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned char *data_00242fc8;
extern int func_00122988(unsigned register_address, const void *source,
    const void *mask, unsigned size);
extern void func_00141560(unsigned mode, float value);
extern unsigned char *data_00242fcc;
extern unsigned char *func_0018d8d4(void);
extern void func_0018d8c4(unsigned char *cursor);
extern void func_00191c94(struct RenderCache *cache, unsigned flags);
extern void func_0018d894(struct W15iRenderer *renderer,
                          struct MatrixObject *render_info, unsigned flags);
#ifdef __cplusplus
}
#endif

#endif
