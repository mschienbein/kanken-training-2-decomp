struct RegistryNode {
    struct RegistryNode *next;
    struct RegistryNode *previous;
};
struct Registry {
    unsigned int count;
    struct RegistryNode sentinel;
};
struct RegistryEntry {
    const void *type;
    struct RegistryNode link;
};
struct RegisteredObject {
    unsigned char reserved[0x88];
    struct RegistryEntry entry;
};

extern struct Registry *func_0017e7c4(void);
extern void func_0017e7bc(struct Registry *registry, struct RegistryEntry *entry);

struct RegisteredObject *func_0014d784(struct RegisteredObject *object)
{
    func_0017e7bc(func_0017e7c4(), &object->entry);
    return object;
}
