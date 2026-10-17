#include "game/selection.h"

/* [INFERENCE] Shared metadata search remains an outlined helper. */
__declspec(noinline) int func_00168670(int identifier, int year)
{
    int index;
    if (year < 0)
        year = 25;
    year += 1988;
    for (index = 0; data_0023fce4[index].header != 0; ++index) {
        const struct MetadataHeader *header = data_0023fce4[index].header;
        if (header->year == year && header->identifier == identifier)
            return index;
    }
    return -1;
}

int func_001674b8(int identifier, int year)
{
    int index = func_00168670(identifier, year);
    if (index < 0)
        return 0;
    return data_0023fce4[index].header->height;
}

int func_001698f4(int identifier, int year)
{
    int index = func_00168670(identifier, year);
    if (index < 0)
        return 0;
    return data_0023fce4[index].header->width;
}
