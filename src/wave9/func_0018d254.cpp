#include "game/resource_stream.h"

static inline Result query_size(Resource *resource, ResourceSize *size)
{
    Resource *object = resource;
    Result result;
    if (size != 0 && object != 0) {
        result = object->get_size(size);
    } else {
        result = Result(0xe0e046bcu);
    }
    return result;
}

extern "C" Result func_0018d254(ResourceSize *size, Resource *resource)
{
    return query_size(resource, size);
}
