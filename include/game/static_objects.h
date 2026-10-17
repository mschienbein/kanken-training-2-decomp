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

extern "C" {

extern const W15fVTable data_002234dc;
W15fObjectA *func_0015f590(W15fObjectA *object);
extern const W15fVTable data_002235f4;
W15fObjectB *func_001aff80(W15fObjectB *object);
}
#endif

#endif
