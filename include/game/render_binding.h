#ifndef GAME_RENDER_BINDING_H
#define GAME_RENDER_BINDING_H
#include "game/runtime.h"
#ifdef __cplusplus
struct RenderDeleteVTable;
struct RenderCompleteLifetime;
extern "C" {
extern const RenderDeleteVTable data_00224544;
extern const RenderDeleteVTable data_002246a0;
extern const RenderDeleteVTable data_00224688;
void func_001d2204(RenderCompleteLifetime *object);
}

/* [INFERENCE] The primary table belongs to the enclosing lifetime; the
 * secondary and tertiary tables belong to its destructed subobjects. */
struct RenderPrimaryLifetime {
    const RenderDeleteVTable *table;
    unsigned char reserved04[0x3c];
};
struct RenderSecondaryLifetime {
    const RenderDeleteVTable *table;
    unsigned char reserved04[0x48];
    ~RenderSecondaryLifetime() { table = &data_002246a0; }
};
struct RenderTertiaryLifetime {
    const RenderDeleteVTable *table;
    ~RenderTertiaryLifetime() { table = &data_00224688; }
};
struct RenderCompleteLifetime {
    RenderPrimaryLifetime primary;
    RenderSecondaryLifetime secondary;
    RenderTertiaryLifetime tertiary;
    ~RenderCompleteLifetime() { primary.table = &data_00224544; }
};

#endif
#endif
