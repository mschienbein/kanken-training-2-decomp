#include "game/resource_font.h"
#include <stdbool.h>
#include <wchar.h>





bool func_0016b80c(struct U8Archive *archive, const wchar_t *path,
                    struct U8Directory *directory)
{
    int index = func_0016ba0c(archive, path);
    const struct U8Node *nodes = archive->nodes;
    if (index >= 0 && (nodes[index].type_and_name & 0xff000000)) {
        directory->archive = archive;
        directory->begin = index;
        directory->next = index + 1;
        directory->end = nodes[index].end_or_size;
        return true;
    }
    return false;
}
