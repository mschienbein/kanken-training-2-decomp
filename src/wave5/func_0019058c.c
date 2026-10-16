struct ServiceState;
struct ServiceObject {
    unsigned handle;
};
extern struct ServiceState *func_0018fcb4(void);
extern unsigned func_0018fa44(struct ServiceState *state, void **memory, unsigned handle);

unsigned func_0019058c(const struct ServiceObject *object)
{
    void *memory;
    unsigned result = func_0018fa44(func_0018fcb4(), &memory, object->handle);
    switch (result >> 31) {
    case 0:
        return 0;
    default:
        return result;
    }
}
