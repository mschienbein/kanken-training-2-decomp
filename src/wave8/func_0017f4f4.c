struct FreeLink {
    struct FreeLink *next;
};

void func_0017f4f4(struct FreeLink **head, struct FreeLink *node)
{
    node->next = *head;
    *head = node;
}
