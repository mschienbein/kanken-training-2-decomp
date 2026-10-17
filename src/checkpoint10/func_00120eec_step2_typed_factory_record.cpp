#include <string.h>
#include "ctr/svc.h"
#include "game/result.h"

struct FactoryObject;
struct FactoryObjectVTable {
    void (*reserved[12])(void);
    void (*destroy)(FactoryObject *object);
};
struct FactoryObject {
    const FactoryObjectVTable *vtable;
    unsigned char reserved[0x130 - sizeof(void *)];
};
struct FactoryInput {
    unsigned int first;
    unsigned int second;
    unsigned char selector;
    unsigned char reserved09[3];
    unsigned int file_id;
};
struct FactoryBuffer {
    void *memory;
    unsigned int size;
    unsigned int flags;
};

struct FactoryInputWords {
    unsigned long long identifier;
    unsigned kind;
    unsigned flags;
};
union FactoryInputRecord {
    FactoryInput input;
    FactoryInputWords words;
};

struct OpenedResource {
    FactoryObject *object;
    unsigned tag;
};

/* INTEGRATOR: move to include/game/factory_service.h; contract copied from
 * scratch/authors/r341/src/func_0012e0e0.cpp's actual self-definition.
 * FileSessionObject is the factory's backing file, not its returned object. */
extern "C" {
CtrResult func_0012e0e0(FactoryObject **out, const FactoryInput *input,
                       unsigned int first_count, unsigned int second_count,
                       FactoryBuffer buffer);
}

extern "C" Result func_00120eec(OpenedResource *out, unsigned kind,
                  unsigned long long identifier, unsigned flags,
                  unsigned first, unsigned second, void *buffer,
                  unsigned size, unsigned last)
{
    FactoryInputRecord record;
    FactoryObject *object;
    FactoryBuffer storage;
    /* [INFERENCE] Two views of the same ABI descriptor: the factory reads
     * a byte selector, while this wrapper forwards its whole argument word. */
    record.words.identifier = identifier;
    record.words.kind = kind;
    record.words.flags = flags;
    storage.memory = buffer;
    storage.size = size;
    storage.flags = last;
    Result result(func_0012e0e0(&object, &record.input, first, second, storage));
    switch (result.failed()) {
    case 0: break;
    default: return result;
    }
    out->object = object;
    out->tag = ~0u;
    return Result(0);
}
