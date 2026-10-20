#include "game/file_handle.h"

/* [INFERENCE] Callers forward both aggregate words. The reserved word is unused
 * by this release implementation and remains part of its caller contract. */
extern "C" Result func_0018fdd4(FileHandle handle)
{
    Result result;
    Interface *object = handle.object;
    if (object != 0) {
        object->vtable->release(object);
        result = Result(0);
    } else {
        result = Result(0xc8804465u);
    }
    return result;
}
