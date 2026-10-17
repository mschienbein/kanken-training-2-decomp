struct ManagedResource;
struct ResourceCallbackState {
    unsigned state;
    unsigned completion;
};

extern void func_00183550(struct ManagedResource *resource);

void func_00152f10(struct ManagedResource *resource, unsigned event,
                   struct ResourceCallbackState *state)
{
    if (event == 0 || event == 1) {
        func_00183550(resource);
        state->completion = 0;
    } else if (event == 2 || event == 3) {
        state->completion = 0;
    }
}
