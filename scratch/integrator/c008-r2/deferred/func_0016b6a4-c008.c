#include "game/resource_font.h"
#include <stdbool.h>
#include <wchar.h>






int func_0016b6a4(struct U8Archive *archive, const wchar_t *name)
{
    struct U8DirectoryEntry entry;
    struct U8Directory directory;
    int result = -1;
    bool available;
    func_0016b80c(archive, data_001cd2ec, &directory);
    available = func_0016b76c(&directory, &entry);
    while (available) {
        if (!entry.is_directory) {
            if (func_0016b678(name, entry.name) == 0) {
                result = entry.index;
                break;
            }
        } else {
            func_0016b860(archive, entry.name);
            result = func_0016b6a4(archive, name);
            func_0016b860(archive, data_001cd2e4);
            if (result != -1)
                break;
        }
        available = func_0016b76c(&directory, &entry);
    }
    /* [INFERENCE] callee identity: inlined by armlink */
    func_001f18c0(&directory);
    return result;
}
