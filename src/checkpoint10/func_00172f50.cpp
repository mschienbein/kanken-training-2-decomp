class OwnedElementW22E {
public:
    virtual ~OwnedElementW22E();
};

struct OwnedElementListW22E {
    unsigned char active;
    signed char kind;
    unsigned char reserved02[6];
    unsigned short first, second;
    int selected;
    int count;
    OwnedElementW22E **entries;
    unsigned remaining;
};

/* INTEGRATOR: move to include/game/owned_element_list.h */
extern "C" {
extern void func_0019e08c(void *allocation);
}

extern "C" void func_00172f50(OwnedElementListW22E *list)
{
    if (list->count > 0 && list->entries != 0) {
        for (int i = 0; i < list->count; ++i) {
            if (list->entries[i] != 0) {
                delete list->entries[i];
                list->entries[i] = 0;
            }
        }
        func_0019e08c(list->entries);
    }
    list->active = 0;
    list->kind = -1;
    list->first = 0;
    list->second = 0;
    list->selected = -1;
    list->count = 0;
    list->entries = 0;
    list->remaining = 0;
}
