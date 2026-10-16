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

extern void func_0018d868(struct Registry *registry, struct RegistryNode *node);

void func_0017e7bc(struct Registry *registry, struct RegistryEntry *entry)
{
    func_0018d868(registry, &entry->link);
}
