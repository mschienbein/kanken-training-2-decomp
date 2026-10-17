struct OffsetListLink {
    void *previous;
    void *next;
};
struct OffsetList {
    void *head;
    void *tail;
    unsigned short count;
    unsigned short link_offset;
};

void func_0012edd4(struct OffsetList *list, void *object)
{
    struct OffsetListLink *link = (struct OffsetListLink *)
        ((unsigned char *)object + list->link_offset);
    if (link->previous == 0) {
        list->head = link->next;
    } else {
        struct OffsetListLink *previous = (struct OffsetListLink *)
            ((unsigned char *)link->previous + list->link_offset);
        previous->next = link->next;
    }
    if (link->next != 0) {
        struct OffsetListLink *next = (struct OffsetListLink *)
            ((unsigned char *)link->next + list->link_offset);
        next->previous = link->previous;
    } else {
        list->tail = link->previous;
    }
    link->previous = 0;
    link->next = 0;
    --list->count;
}
