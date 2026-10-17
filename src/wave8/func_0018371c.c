struct RangeLink {
    struct RangeLink *next;
};

struct RegionList {
    unsigned reserved00[2];
    struct RangeLink *head;
};

extern void func_00183724(struct RangeLink **head, void *memory, unsigned size);

void func_0018371c(struct RegionList *list, void *memory, unsigned size)
{
    func_00183724(&list->head, memory, size);
}
