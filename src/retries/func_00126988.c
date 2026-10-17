#include "game/package_file.h"
#include "game/archive.h"



static __inline bool offset_within(unsigned offset, unsigned length) { return offset < length; }

static __inline bool operation_failed(unsigned result) { return result >> 31; }

struct WordPair *func_00126988(struct WordPair *out,
                                         const struct PackageView *view,
                                         unsigned *transferred,
                                         void *destination, unsigned count,
                                         unsigned offset)
{
    struct PackageFile *file = view->file;
    unsigned result;
    if (offset_within(offset, file->path_length)) {
        unsigned available = file->path_length - offset;
        if (available < count)
            count = available;
        result = func_0012d4d0(destination, 1,
                              file->handle.first | ((unsigned long long)file->handle.second << 32),
                              file->path + offset, count);
        if (!operation_failed(result)) {
            result = 0;
            *transferred = count;
        }
    } else {
        result = *transferred = 0;
    }
    return func_00194a34(out, result);
}
