#include "game/archive.h"

struct PackageAttribute;
struct PackageFile {
    struct WordPair handle;
    unsigned char reserved08[0x10];
    const struct PackageAttribute *attributes;
    unsigned attribute_count;
    unsigned reserved20;
    unsigned short *path;
    unsigned path_length;
    unsigned reserved2c;
};
struct PackageDirectory {
    struct PackageFile *entries;
    unsigned count;
};
struct PackageView {
    struct PackageFile *file;
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
