#ifndef GAME_MATERIAL_STORAGE_H
#define GAME_MATERIAL_STORAGE_H

#include <preinclude.h>
#include <nw/lyt/lyt_Material.h>

/* [INFERENCE] Read-only resource prefix; matching capacity representation does
 * not establish identity with nw::lyt::Material or a native method overload. */
struct PackedResource {
    unsigned char reserved00[0x2c];
    nw::lyt::internal::MatMemNums capacity;
    unsigned reserved30;
    void *data;
};

extern "C" const unsigned char *func_001f169c(const PackedResource *resource);

#endif
