#include <stddef.h>
#include "game/waitable_handle.h"
#include "game/resource_cleanup.h"



LargeResourceElement *func_001c3100(LargeResourceElement *element)
{
    CompositeResource *parts = reinterpret_cast<CompositeResource *>(element);
    FrameResource *frame = func_001d245c(&parts->frame);
    ResourceCleanup *cleanup = reinterpret_cast<ResourceCleanup *>(
        reinterpret_cast<unsigned char *>(frame) -
        (offsetof(CompositeResource, frame) - offsetof(CompositeResource, cleanup)));
    cleanup = func_001d28f4(cleanup);
    return reinterpret_cast<LargeResourceElement *>(
        reinterpret_cast<unsigned char *>(cleanup) - offsetof(CompositeResource, cleanup));
}
