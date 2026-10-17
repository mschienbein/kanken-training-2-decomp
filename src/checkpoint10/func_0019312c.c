#include "game/package_file.h"
#include "game/archive.h"

struct PackageAttribute;
struct PackageDirectory {
    struct PackageFile *entries;
    unsigned count;
};
struct PackageArchive {
    void *reserved00;
    struct PackageDirectory *directory;
};

void func_0019312c(struct PackageView *out, const struct PackageArchive *archive,
                   unsigned index)
{
    struct PackageDirectory *directory;
    out->file = 0;
    directory = archive->directory;
    /* ORIGINAL BUG: index == count is accepted and produces a one-past entry. */
    out->file = (directory && index <= directory->count) ? &directory->entries[index] : 0;
}
