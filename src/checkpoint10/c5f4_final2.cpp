#include "game/functions.h"
#include "game/matrix.h"
#include "game/pool_list.h"

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
/* INTEGRATOR: differs from include/game/screen.h (DisplayPane) and
 * include/game/layout.h (LayoutPane): expanded pane view with recovered
 * lists, transform matrices, packed mode, name and user-data fields. */
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

/* INTEGRATOR: identical to MatrixIdentityW8C / matrix34Identity() in
 * src/wave8/func_00180234.cpp (same COMDAT statics, bound in config/names.json);
 * move both to include/game/matrix.h. */
struct MatrixIdentityW8C : Matrix34 {
    MatrixIdentityW8C()
    {
        elements[0] = 1.0f; elements[1] = 0.0f; elements[2] = 0.0f; elements[3] = 0.0f;
        elements[4] = 0.0f; elements[5] = 1.0f; elements[6] = 0.0f; elements[7] = 0.0f;
        elements[8] = 0.0f; elements[9] = 0.0f; elements[10] = 1.0f; elements[11] = 0.0f;
    }
};

__forceinline const Matrix34 *matrix34Identity()
{
    /* [INFERENCE] callee identity: the guard-release call was inlined by armlink. */
    static MatrixIdentityW8C identity_value;
    return &identity_value;
}

extern "C" void func_001405f4(SceneNode *node)
{
    node->parent = 0;
    node->flags = 0;
    node->next = 0;
    func_00195334(&node->local, matrix34Identity());
    func_00195334(&node->world, matrix34Identity());
}
