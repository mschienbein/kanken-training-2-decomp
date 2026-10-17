#ifndef GAME_ENTRY_OBJECT_H
#define GAME_ENTRY_OBJECT_H



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct EntryObject;

struct EntryObject {
    const void *vtable;
    void *first;
    void *second;
    void *third;
    unsigned kind;
    unsigned short count;
    short selected;
};

extern "C" {
extern const unsigned data_00223d98[];
EntryObject *func_00113e78(EntryObject *object);
EntryObject *func_0010cc94(EntryObject *object);
}
#endif

#endif
