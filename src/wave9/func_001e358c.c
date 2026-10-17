#include <stdint.h>

struct DeferredResource {
    unsigned reserved00;
    uintptr_t resource;
    int index;
};
struct DeferredRequest {
    int state;
    int resourceIndex;
    unsigned char *data;
};
extern const struct DeferredResource data_0023fce4[];
extern int func_0019efd0(void);
extern unsigned char *func_001971a0(uintptr_t resource, int index);

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
