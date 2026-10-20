struct FirstReadinessState {
    unsigned char reserved00[0x9c];
    unsigned char enabled;
    unsigned char reserved9d[0x84];
    unsigned char forced;
    unsigned char reserved122[0x16a];
    unsigned char phase;
};

int func_001f18d0(const struct FirstReadinessState *state)
{
    if (state->forced != 0 || (state->enabled != 0 && state->phase >= 3))
        return 1;
    return 0;
}
