struct StreamPlayer;
struct WaveBufferInfo;
extern int func_00153af8(struct StreamPlayer *player, int success,
                         const struct WaveBufferInfo *buffer, int prefetch,
                         unsigned block, unsigned channel_stride);

int func_00150c98(struct StreamPlayer *player, int success, const struct WaveBufferInfo *buffer)
{
    return func_00153af8(player, success, buffer, 0, 0, 0);
}
