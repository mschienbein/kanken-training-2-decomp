#include "game/runtime.h"

class ResourceReleasable {
public:
    virtual ~ResourceReleasable();
};


extern "C" void func_0019cf20(ResourceHandle *resource)
{
    if (data_0024223c == 0 || data_0024223c == 1)
        return;
    if (data_0024223c != 2)
        return;
    if (ResourceHandle *handle = resource) {
        if (handle->kind == 1) {
            if (handle->allocation)
                func_0019dc20(handle->allocation);
        } else if (handle->kind == 2) {
            if (handle->allocation)
                func_0019dc20(handle->allocation);
        } else if (handle->kind == 3) {
            if (handle->allocation)
                func_0019dc20(handle->allocation);
        } else if (handle->kind == 4) {
            if (handle->allocation)
                func_0019dc20(handle->allocation);
        } else if (handle->kind == 5) {
            if (handle->allocation)
                func_0019dc20(handle->allocation);
        }
        handle->active = 0;
        delete handle->object;
    }
}
