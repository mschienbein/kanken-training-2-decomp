#include "game/resource_font.h"
#include <stdbool.h>
#include <wchar.h>




bool func_0016b860(struct U8Archive *archive, const wchar_t *path)
{
    int index = func_0016ba0c(archive, path);
    const struct U8Node *nodes = archive->nodes;
    if (index >= 0 && (nodes[index].type_and_name & 0xff000000)) {
        archive->current_directory = index;
        return true;
    }
    return false;
}
