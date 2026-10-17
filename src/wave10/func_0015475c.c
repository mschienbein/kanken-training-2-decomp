struct WaveChannel;
struct AdpcmLoopState {
    short predictor_scale;
    short history1;
    short history2;
};
struct WaveDataBlock {
    const void *data;
    unsigned sample_count;
    const struct AdpcmLoopState *loop;
    unsigned reserved0c[3];
};
struct StreamChannel {
    void *allocation;
    struct WaveChannel *channel;
    struct WaveDataBlock blocks[32];
    struct AdpcmLoopState loop_states[32];
};
extern void func_0017da08(struct WaveChannel *channel, unsigned index,
                          const struct WaveDataBlock *block, int update);

void func_0015475c(struct StreamChannel *stream, const struct WaveDataBlock *block, int update)
{
    if (stream->channel != 0)
        func_0017da08(stream->channel, 0, block, update);
}
