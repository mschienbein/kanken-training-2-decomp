#ifndef GAME_SCENE_RUNTIME_H
#define GAME_SCENE_RUNTIME_H

struct SceneVec3;
struct SceneVec2;
union SceneMode;
struct SceneNode;
struct SceneRecord;

#include "game/matrix.h"
#include "game/pool_list.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
namespace nw { namespace gfx { class FragmentLight; } }




struct SceneVec3 { float x, y, z; };
struct SceneVec2 { float x, y; };
union SceneMode {
    unsigned char byte;
    struct {
        unsigned char kind : 4;
        unsigned char global : 1;
        unsigned char reserved : 3;
    } bits;
};
struct SceneNode {
    const void *const *vtable;
    unsigned value04;
    unsigned value08;
    struct SceneNode *parent;
    struct PoolList children;
    struct PoolList links;
    struct SceneVec3 position;
    struct SceneVec3 angles;
    struct SceneVec2 scale;
    struct SceneVec2 offset;
    struct Matrix34 local;
    struct Matrix34 world;
    struct SceneNode *next;
    unsigned char alpha;
    unsigned char inherited_alpha;
    union SceneMode mode;
    unsigned char flags;
    char name[17];
    char secondary_name[9];
    unsigned char reservedd2[2];
};

extern "C" {

extern const void *const data_002243f8[];
struct SceneNode *func_0016b120(struct SceneNode *node, const struct SceneRecord *record);
struct SceneNode *func_001cfd24(struct SceneNode *node, const struct SceneRecord *record);
void func_001c5ea0(nw::gfx::FragmentLight *node);
void func_001405f4(struct SceneNode *node);
}
#endif

#endif
