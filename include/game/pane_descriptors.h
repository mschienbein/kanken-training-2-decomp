#ifndef GAME_PANE_DESCRIPTORS_H
#define GAME_PANE_DESCRIPTORS_H

#include "game/render_parameter.h"
#include "game/property_descriptor.h"


struct PaneRange {
    int identifier;
    int kind;
    float minimum_x;
    float maximum_x;
    float minimum_y;
    float maximum_y;
    unsigned int reserved18;
#ifdef __cplusplus
    PaneRange(int id, int dimensions, float max_x, float max_y)
    {
        identifier = id;
        kind = dimensions;
        minimum_x = 0.0f;
        maximum_x = max_x;
        minimum_y = 0.0f;
        maximum_y = max_y;
        /* [INFERENCE] The trailing allocation word is not initialized here. */
    }
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct RenderParameter *bss_002911bc[12];
extern struct RenderParameter *bss_002911ec[6];
extern struct PaneRange *data_0023ed08[2];
extern struct NameTable *bss_0024716c[12];
extern struct NameTable *bss_00290cf8[8];
extern struct NameTable *bss_00247140[4];
extern struct NameTable *bss_00290d54[7];
extern struct NameTable *bss_00290d8c[7];
extern struct NameTable *bss_00290dd8[12];
extern void func_002062b4(void);
extern void func_00209bcc(void);
extern void func_0020e038(void);
extern void func_00210084(void);
extern void func_00210620(void);
extern void func_00213798(void);
extern void func_00213d3c(void);
#ifdef __cplusplus
}
#endif

#endif
