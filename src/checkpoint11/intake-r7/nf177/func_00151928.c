/* [INFERENCE] Descriptive field names for the observed phase-accumulator layout. */
struct PhaseAccumulator {
    unsigned reserved00;
    float rate;
    unsigned delay;
    unsigned char reserved0c[2];
    unsigned char initial;
    unsigned char reserved0f;
    unsigned elapsed;
    float phase;
    unsigned reserved18;
    unsigned char initialized;
    unsigned char wrapped;
};

void func_00151928(struct PhaseAccumulator *state, int delta)
{
    if (state->elapsed < state->delay) {
        if (state->elapsed + delta <= state->delay) {
            state->elapsed += delta;
            return;
        }
        delta -= state->delay - state->elapsed;
        state->elapsed = state->delay;
    }
    if (state->rate > 0.0f) {
        if (!state->initialized) {
            state->initialized = 1;
            state->phase = state->initial * (1.0f / 127.0f);
        }
        state->phase += state->rate * delta * 0.001f;
        if (state->phase >= 1.0f) {
            state->wrapped = 1;
        } else {
            state->wrapped = 0;
        }
        state->phase -= (int)state->phase;
    }
}
