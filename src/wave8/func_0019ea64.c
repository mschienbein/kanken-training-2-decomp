struct SynchronizationState {
    volatile unsigned state;
};

struct SynchronizationOwner {
    const void *vtable;
    struct SynchronizationState synchronization;
};

extern void func_0019ea6c(struct SynchronizationState *state);

void func_0019ea64(struct SynchronizationOwner *object)
{
    func_0019ea6c(&object->synchronization);
}
