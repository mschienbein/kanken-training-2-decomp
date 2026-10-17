struct TransformNode;
struct LightList {
    char reserved0[36];
    struct TransformNode **entries;
    char reserved40[116];
    int index;
    unsigned reserved160;
    unsigned count;
};

void func_00116954(struct LightList *object)
{
    object->index = -1;
}
