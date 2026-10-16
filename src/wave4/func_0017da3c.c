struct RecordState {
    unsigned int words[4];
    union {
        unsigned int word;
        struct {
            unsigned char first;
            unsigned char second;
            unsigned char third;
            unsigned char fourth;
        } bytes;
    } flags;
    unsigned int value;
};

void func_0017da3c(struct RecordState *state)
{
    state->words[0] = 0;
    state->words[1] = 0;
    state->words[2] = 0;
    state->words[3] = 0;
    state->flags.word = 0;
    state->value = 0;
    state->flags.bytes.second = 0;
}
