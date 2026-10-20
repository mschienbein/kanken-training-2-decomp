#ifndef GAME_STATIC_OBJECTS_H
#define GAME_STATIC_OBJECTS_H

struct W15fObjectA;
struct W15fObjectB;
struct W15fVTable;



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */


struct W15fObjectA {
    const W15fVTable *vtable;
};
struct W15fObjectB {
    const W15fVTable *vtable;
};
/* [INFERENCE] Only this registered object's vtable word is exposed. */
struct W19RegisteredObject {
    const void *const *vtable;
};

extern "C" {

extern const W15fVTable data_002234dc;
W15fObjectA *func_0015f590(W15fObjectA *object);
extern const W15fVTable data_002235f4;
W15fObjectB *func_001aff80(W15fObjectB *object);
extern struct W19RegisteredObject data_002426cc;
extern const void *const data_00224dcc[];
void func_001f5780(void);
}
#endif

#endif
