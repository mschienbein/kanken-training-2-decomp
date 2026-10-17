struct W15iRecordCache {
    unsigned words[3];
    unsigned char active;
};

extern "C" void func_001d482c(W15iRecordCache *state)
{
    if (state->active) {
        state->words[0] = 0;
        state->words[1] = 0;
        state->words[2] = 0;
        state->active = 0;
    }
}
