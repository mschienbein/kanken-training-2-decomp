#include "game/functions.h"

struct W19RegisteredObject { const void *vtable; };
/* INTEGRATOR: move to include/game/static_registry.h */
extern "C" {
extern struct W19RegisteredObject data_002426cc;
extern struct W19RegisteredObject data_002426d0;
extern const void *const data_00224dcc[];
extern const void *const data_00224db4[];
extern void func_00164ebc(void *object, ObjectDestructor destroy, void (*image_base)(void));
/* INTEGRATOR: differs from src/wave3/empty_functions.c: registration callbacks
 * receive their registered object, even though these destructors ignore it. */
extern void func_001e3818(struct W19RegisteredObject *object);
/* INTEGRATOR: differs from src/wave3/empty_functions.c: registration callbacks
 * receive their registered object, even though these destructors ignore it. */
extern void func_001e37dc(struct W19RegisteredObject *object);
void func_001f5780(void);
}

void func_001f5780(void)
{
    struct W19RegisteredObject *object = &data_002426cc;
    object->vtable = data_00224dcc;
    /* [INFERENCE] callee identity: inlined by armlink */
    func_00164ebc(object, (ObjectDestructor)func_001e3818, func_00100000);
    object = &data_002426d0;
    object->vtable = data_00224db4;
    func_00164ebc(object, (ObjectDestructor)func_001e37dc, func_00100000);
}
