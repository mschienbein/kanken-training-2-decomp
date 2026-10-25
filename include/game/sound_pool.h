#ifndef GAME_SOUND_POOL_H
#define GAME_SOUND_POOL_H
/* [INFERENCE] Caller-visible channel allocation record; semantic names follow use. */
struct ChannelBuffer {
    void *base;
    unsigned size, channel_size, channel_count, allocated, flags;
};
#ifdef __cplusplus
extern "C" {
#endif
void func_00147390(struct ChannelBuffer *buffer, void *storage, unsigned size, unsigned channels);
#ifdef __cplusplus
}
#endif
#endif
