#ifndef GAME_PANE_DESCRIPTORS_H
#define GAME_PANE_DESCRIPTORS_H

struct PanePlacement;

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


#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */

struct PanePlacement {
    float x, y, depth;
    PanePlacement(float depth_value, float x_value, float y_value)
        : x(x_value), y(y_value), depth(depth_value) {}
};

extern "C" {
extern NameTable *bss_00244378[30];
void func_00206afc(void);
void func_00170e58(Screen *screen, int index, int enabled);
extern PanePlacement *bss_00246f7c[11];
void func_0020fa08(void);
void func_00212b60(void);
void func_002131b4(void);
void func_00214794(void);
void func_002153f4(void);
}
#endif

#endif
