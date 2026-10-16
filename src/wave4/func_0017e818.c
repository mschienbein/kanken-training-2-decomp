struct TimingState {
    float elapsed;
    float duration;
    unsigned int count;
    unsigned char active;
    unsigned char paused;
    unsigned char complete;
};

void func_0017e818(struct TimingState *state)
{
    state->elapsed = 0.0f;
    state->duration = 6.25f;
    state->count = 0;
    state->active = 1;
    state->paused = 0;
    state->complete = 0;
}
