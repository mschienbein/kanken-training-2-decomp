#include "game/graphics_resources.h"
#include "game/functions.h"
#include "game/static_objects.h"

/* [INFERENCE] Registration and destructor choices at erased original calls
 * are source hypotheses, not proof of historical callback invocation or a
 * complete native class. The concrete callbacks retain the object input. */


void func_001f5780(void)
{
    struct W19RegisteredObject *object = &data_002426cc;
    object->vtable = data_00224dcc;
    /* [INFERENCE] callee identity: inlined by armlink */
    func_00164ebc(object, func_001e3818, func_00100000);
    GraphicsHeap *heap = &data_002426d0;
    heap->vtable = &data_00224db4;
    func_00164ebc(heap, func_001e37dc, func_00100000);
}
