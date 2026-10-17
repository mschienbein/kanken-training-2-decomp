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
struct PackageView {
    struct PackageFile *file;
};

unsigned func_00193120(const struct PackageView *view)
{
    return view->file->path_length;
}
