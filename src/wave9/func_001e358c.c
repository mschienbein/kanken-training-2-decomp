#include "game/functions.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/resource_tables.h"
struct DeferredRequest {
    int state;
    int resourceIndex;
    unsigned char *data;
};


void func_001e358c(struct DeferredRequest *request)
{
    if (request->state) {
        if (request->state == 1 && !func_0019efd0()) {
            uintptr_t resource = data_0023fce4[request->resourceIndex].resource;
            int index = data_0023fce4[request->resourceIndex].index;
            request->data = func_001971a0(resource, index);
            request->state = 0;
        }
    }
}
