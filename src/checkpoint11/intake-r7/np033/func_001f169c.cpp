#include "game/material_storage.h"

const unsigned char *func_001f169c(const PackedResource *resource)
{
    const nw::lyt::internal::MatMemNums capacity = resource->capacity;
    return nw::lyt::internal::ConvertOffsToPtr<unsigned char>(resource->data,
        sizeof(nw::lyt::TexMap) * capacity.texMap +
        sizeof(nw::lyt::TexSRT) * capacity.texSRT);
}
