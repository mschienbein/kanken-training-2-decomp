#include "game/resource_collection.h"

void func_001cfe24(ResourceCollection *collection)
{
    collection->vtable = &data_00224480;
    collection->clear();
    if (collection->allocation) {
        func_00191aec(collection->allocation);
        collection->allocation = 0;
    }
    func_001852f4(collection);
}
