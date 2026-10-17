#include "game/scene_runtime.h"
#include "game/functions.h"
#include "game/matrix.h"
#include "game/pool_list.h"

/* INTEGRATOR: differs from include/game/screen.h (DisplayPane) and
 * include/game/layout.h (LayoutPane): expanded pane view with recovered
 * lists, transform matrices, packed mode, name and user-data fields. */

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
