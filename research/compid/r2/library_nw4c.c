/* Anchored by CFNT/FINF/CMAP font parsing and CLYT layout parsing callers. */
unsigned short func_001db054(const unsigned short **position)
{
    return *(*position)++;
}
unsigned char func_001db068(const unsigned char **position)
{
    return *(*position)++;
}
struct Link { struct Link *next, *previous; };
struct List { int reserved; struct Link end; };
extern void func_00191154(struct List *list, struct Link *before, struct Link *node);
void func_001cc1d4(struct List *list, struct List *node)
{
    func_00191154(list, &list->end, &node->end);
}
