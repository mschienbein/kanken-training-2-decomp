struct TimingState {
    float elapsed;
    float duration;
    unsigned int count;
    unsigned char active;
    unsigned char paused;
    unsigned char complete;
};

extern void func_0017e818(struct TimingState *state);

struct TimingState *func_001d3898(struct TimingState *state)
{
    func_0017e818(state);
    return state;
}
