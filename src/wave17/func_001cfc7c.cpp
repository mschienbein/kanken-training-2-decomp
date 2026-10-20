#include <stddef.h>
#include "game/pool_list.h"
#include "game/runtime.h"
#include "game/screen.h"
#include "game/layout_lifecycle.h"



void *W17TexturePane::destroy()
{
    W17TexturePane *pane = this;
    pane->base.vtable = &data_00224370;
    W17Material *resource = pane->resource;
    if (resource && !(resource->flags & 1)) {
        resource->vtable->destroy(resource);
        func_00191aec(resource);
        pane->resource = 0;
    }
    if (pane->text) {
        func_00191aec(pane->text);
        pane->text = 0;
        pane->length = 0;
        pane->capacity = 0;
    }
    if (pane->storage) {
        W17StorageHeader *header = static_cast<W17StorageHeader *>(pane->storage);
        if (header->flags & 8)
            func_0016b260(header);
        else
            func_00191aec(header);
        pane->storage = 0;
    }
    return func_0016a998(pane);
}
