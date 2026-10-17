#include "game/resource_font.h"
#include <stdbool.h>
#include <wchar.h>





bool func_0016b76c(struct U8Directory *directory, struct U8DirectoryEntry *entry)
{
    struct U8Archive *archive = directory->archive;
    unsigned index = directory->next;
    const struct U8Node *nodes = archive->nodes;
    for (;;) {
        if (!(directory->begin < index && index < directory->end))
            return false;
        entry->index = index;
        entry->archive = archive;
        entry->is_directory = (nodes[index].type_and_name & 0xff000000) != 0;
        entry->name = (const wchar_t *)((const unsigned char *)archive->names +
                                        (nodes[index].type_and_name & 0xffffff));
        if (entry->name[0] != L'.' || entry->name[1] != 0)
            break;
        ++index;
    }
    directory->next = (nodes[index].type_and_name & 0xff000000)
                          ? nodes[index].end_or_size : index + 1;
    return true;
}
