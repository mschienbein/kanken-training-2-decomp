struct TimingState {
    float elapsed;
    float duration;
    unsigned int count;
    unsigned char active;
    unsigned char paused;
    unsigned char complete;
};
struct ExtendedTimingState {
    struct TimingState timing;
    unsigned int value;
    float start;
    float end;
    unsigned char enabled;
    unsigned char finished;
};

extern void func_0017e818(struct TimingState *state);

struct ExtendedTimingState *func_001d9920(struct ExtendedTimingState *state)
{
    func_0017e818(&state->timing);
    state->value = 0;
    state->start = 0.0f;
    state->end = 1.0f;
    state->enabled = 0;
    state->finished = 0;
    return state;
}
