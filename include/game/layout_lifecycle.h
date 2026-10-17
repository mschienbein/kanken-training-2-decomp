#ifndef GAME_LAYOUT_LIFECYCLE_H
#define GAME_LAYOUT_LIFECYCLE_H

struct W17MaterialVTable;
struct W17Material;
struct W17LayoutVTable;
struct W17Layout;
struct W17LayoutChild;
struct W17TexturePane;
struct W17StorageHeader;

#include <stddef.h>
#include "game/pool_list.h"
#include "game/runtime.h"
#include "game/screen.h"
#include "game/resource_instance.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */










struct W17MaterialVTable {
    void (*destroy)(W17Material *);
    void (*reserved04)(W17Material *);
    void (*reserved08)(W17Material *);
    void (*unbind)(W17Material *, void *animation);
};
struct W17Material {
    const W17MaterialVTable *vtable;
    unsigned char reserved04[0x49];
    unsigned char flags;
};
struct W17LayoutVTable {
    void (*destroy)(W17Layout *);
    void (*reserved04)(W17Layout *);
    void (*reserved08)(W17Layout *);
    void (*reserved0c)(W17Layout *);
    void (*reserved10)(W17Layout *);
    void (*reserved14)(W17Layout *);
    void (*reserved18)(W17Layout *);
    void (*reserved1c)(W17Layout *);
    void (*reserved20)(W17Layout *);
    unsigned char (*material_count)(W17Layout *);
    W17Material *(*material)(W17Layout *, unsigned char);
};
struct W17Layout {
    const W17LayoutVTable *vtable;
    unsigned char reserved04[0x0c];
    PoolList children;
    PoolList links;
};
struct W17LayoutChild {
    const W17LayoutVTable *vtable;
    PoolLink link;
    unsigned char reserved0c[0xab];
    unsigned char flags;
};
struct W17TexturePane {
    void *destroy();
    W17Layout base;
    unsigned char reserved28[0xac];
    void *text;
    unsigned char reservedd8[0x20];
    unsigned short length;
    unsigned short capacity;
    unsigned reservedfc;
    W17Material *resource;
    void *storage;
};
struct W17StorageHeader {
    unsigned char reserved00[9];
    unsigned char flags;
};

extern "C" {

void func_0016b260(void *allocation);


void func_00191aec(void *allocation);
void *func_0016a998(void *object);
void func_0016aa74(DisplayVertexArray *buffer);
void func_001cf67c(W17TexturePane *pane);
extern const W17LayoutVTable data_00224370;
void func_001cfbcc(W17TexturePane *pane);
}
#endif

#endif
