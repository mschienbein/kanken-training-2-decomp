struct ScalarState {
    float first;
    float second;
    unsigned count;
    unsigned flags;
};

void *func_0014c2cc(void *storage)
{
    struct ScalarState *state = storage;
    state->first = 0.0f;
    state->second = 0.0f;
    state->count = 0;
    state->flags = 0;
    return storage;
}
