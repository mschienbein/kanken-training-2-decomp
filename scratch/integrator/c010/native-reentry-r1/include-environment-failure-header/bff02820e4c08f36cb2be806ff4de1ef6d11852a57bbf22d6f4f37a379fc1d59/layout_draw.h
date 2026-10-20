#ifndef GAME_LAYOUT_DRAW_H
#define GAME_LAYOUT_DRAW_H

struct AudioChannelState;
struct ChannelFlagWord;
struct LayoutAuxEntry;
struct LayoutColorWord;
struct LayoutDrawPane;
struct LayoutDrawState;
struct LayoutDrawStorage;
struct LayoutGeometryData;
struct LayoutParameterBase;
struct LayoutRenderContext;
struct LayoutSubobject;
struct LayoutSurface;
struct LayoutSurfaceVTable;
struct Matrix34;
struct WidgetPosition;

#include "game/matrix.h"
#include "game/runtime.h"
#include "game/screen.h"

#ifdef __cplusplus
#include <nw/lyt/lyt_Pane.h>
/* [INFERENCE] Target root prefix only, not a complete native Layout alias.
 * The original +0x10 member is passed unchanged to Pane::DrawCommands. */
struct LayoutCommandRoot {
    unsigned char reserved00[0x10];
    nw::lyt::Pane *root;
};
extern "C" void func_00196c04(LayoutRenderContext *renderer,
    LayoutCommandRoot *root, LayoutDrawInfo *info);
#endif

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
struct LayoutColorWord {
    unsigned int word;
    LayoutColorWord();
};
#endif

struct LayoutGeometryData {
    unsigned int reserved00;
    unsigned short count;
    unsigned short capacity;
    unsigned char active;
    unsigned char flags;
    unsigned short reserved0a;
    unsigned int reserved0c;
    unsigned int *current;
    unsigned int *alternate;
    unsigned int *external;
    unsigned int word_count;
    unsigned int reserved20;
    unsigned int state24;
};

struct LayoutSurfaceVTable {
    unsigned char reserved00[8];
    int (*width)(struct LayoutSurface *);
    int (*height)(struct LayoutSurface *);
};

struct LayoutSurface { const struct LayoutSurfaceVTable *vtable; };

#ifdef __cplusplus
struct LayoutParameterBase {
    unsigned int color0;
    unsigned int color1;
    LayoutColorWord current[4];
    LayoutColorWord colors[2];
    unsigned char mode;
    unsigned char reserved21[3];
    float scale_x;
    float scale_y;
    float offset_x;
    float offset_y;
    float offset_z;
    float value38;
    LayoutSurface *surface;
    unsigned int value40;
    LayoutGeometryData *geometry;
    unsigned char enabled;
    unsigned char alpha;
    unsigned short reserved4a;
    LayoutParameterBase();
};
#endif

#ifdef __cplusplus
struct LayoutDrawState : LayoutParameterBase {
    float max_depth;
    float value50;
    float value54;
    unsigned int kind;
    unsigned int flags;
    const unsigned int *table;
    LayoutDrawState();
};
#endif

#ifdef __cplusplus
struct LayoutAuxEntry {
    unsigned int reserved00[5];
    LayoutAuxEntry();
};
#endif

#ifdef __cplusplus
struct LayoutSubobject {
    unsigned int reserved00[2];
    LayoutAuxEntry entries[4];
    unsigned char reserved58[0xe4 - 0x58];
    unsigned int valuee4;
    float valuee8;
    unsigned char flagec;
    unsigned char reserveded[3];
    LayoutSubobject();
};
#endif

#ifdef __cplusplus
struct LayoutDrawStorage {
    unsigned int first;
    unsigned int second;
    LayoutSubobject subobject;
    LayoutDrawState state;
    unsigned int value15c;
    unsigned int value160;
    unsigned char reserved164[0x2e0 - 0x164];
    unsigned char flag2e0;
    LayoutDrawStorage();
};
#endif

struct LayoutDrawPane {
    const void *vtable;
    unsigned char reserved04[0x44];
    struct WidgetPosition size;
    unsigned char reserved50[0x30];
    struct Matrix34 matrix;
    unsigned char reservedb0[5];
    unsigned char alpha;
    unsigned char origin;
    unsigned char flags;
    unsigned char reservedb8[0x1c];
    const void *source;
    unsigned int colors[2];
    struct LayoutSurface *surface;
    struct WidgetPosition surface_size;
    float valueec;
    float valuef0;
    const unsigned int *table;
    unsigned short reservedf8;
    unsigned short source_id;
    unsigned char alignment;
    unsigned char mode;
    unsigned char pending;
    unsigned char reservedff;
    unsigned int reserved100;
    struct LayoutGeometryData *geometry;
};

struct LayoutRenderContext {
    unsigned char reserved00[0x25];
    unsigned char dirty;
    unsigned char reserved26[0x66c - 0x26];
    struct Matrix34 *matrix;
    unsigned char reserved670[0x14];
    unsigned int *first_command;
    unsigned int *last_command;
    unsigned int *patch_count;
    unsigned char reserved690[0x10];
    unsigned char flaga0;
    unsigned char flaga1;
    unsigned char flaga2;
};

struct ChannelFlagWord {
    unsigned short reserved : 2;
    unsigned short mode : 2;
    unsigned short remaining : 12;
};

#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned int data_002438dc[];
extern void func_001cf504(const struct LayoutDrawPane *pane, struct LayoutDrawState *state);
extern void func_001da0e0(struct LayoutDrawState *state);
extern void func_00216670(struct LayoutDrawState *state, const void *source, unsigned short id);
extern void func_001da51c(struct LayoutRenderContext *context, struct LayoutDrawState *state);
extern void *func_00125b50(void *object);
extern struct WidgetPosition func_0016637c(const struct LayoutDrawPane *pane);
extern void func_0017a558(struct LayoutParameterBase *state);
extern void func_001c5978(struct AudioChannelState *state, unsigned int value);
extern void func_001da0fc(struct LayoutParameterBase *state, float width, float height);
extern void func_001cf6dc(struct LayoutDrawPane *pane, struct LayoutRenderContext *context);
#ifdef __cplusplus
}
#endif

#endif
