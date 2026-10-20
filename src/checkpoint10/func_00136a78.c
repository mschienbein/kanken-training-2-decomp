/* ORIGINAL LAYOUT: intrusive list of objects; each object embeds a link at list->link_offset. */
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

void func_00136a78(struct OffsetList *list, void *object)
{
    struct OffsetListLink *link;
    if (list->head == 0) {
        link = (struct OffsetListLink *)((unsigned char *)object + list->link_offset);
        link->next = 0;
        link->previous = 0;
        list->head = object;
        list->tail = object;
        list->count++;
    } else {
        link = (struct OffsetListLink *)((unsigned char *)object + list->link_offset);
        link->previous = list->tail;
        link->next = 0;
        ((struct OffsetListLink *)((unsigned char *)list->tail + list->link_offset))->next = object;
        list->tail = object;
        list->count++;
    }
}
