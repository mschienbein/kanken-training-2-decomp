#include "game/result.h"

#include "game/resource.h"

struct Path {
    unsigned type;
    const void *data;
    unsigned size;
};


struct ArchiveInterface;

struct ArchiveVTable {
    Result (*open)(ArchiveInterface *, Resource **, const Path *, unsigned);
};

struct ArchiveInterface {
    const ArchiveVTable *vtable;
};

static inline Result open_archive(ArchiveInterface *archive, Resource **output,
                                  const Path *path, unsigned flags)
{
    Result result;
    Resource *resource;
    if (archive != 0) {
        result = archive->vtable->open(archive, &resource, path, flags);
        switch (result.failed()) {
        case 0:
            *output = resource;
            result = Result(0);
            break;
        }
    } else {
        result = Result(0xc8804465u);
    }
    return result;
}

extern "C" Result func_0018fd20(Resource **output, ArchiveInterface *archive,
                              unsigned, const Path *path, unsigned flags)
{
    return open_archive(archive, output, path, flags);
}
