struct ElementCollection;
extern void func_001e8c74(struct ElementCollection *collection, int index);

void func_001e9e7c(struct ElementCollection *collection)
{
    int index;
    for (index = 0; index < 20; ++index)
        func_001e8c74(collection, index);
}
