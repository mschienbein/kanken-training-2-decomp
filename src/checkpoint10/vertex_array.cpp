#include <preinclude.h>
#include <nw/lyt/lyt_Layout.h>
#include "game/screen.h"

/* [INFERENCE] The no-argument vector constructor leaves coordinates untouched. */
inline nn::math::VEC2::VEC2() {}

/* INTEGRATOR: move to include/game/screen.h. The game-facing count is unsigned;
 * the narrower native TexCoordAry::Reserve declaration is not substituted. */
extern "C" {
void func_0017f7f8(DisplayVertexArray *vertices, unsigned capacity);
}

static void free_coordinates(DisplayVertexArray *vertices)
{
    if (vertices->entries) {
        nw::lyt::Layout::FreeMemory(vertices->entries);
        vertices->entries = 0;
        vertices->capacity = 0;
        vertices->count = 0;
    }
}

void func_0017f7f8(DisplayVertexArray *vertices, unsigned capacity)
{
    if (vertices->capacity < capacity) {
        free_coordinates(vertices);
        nn::math::VEC2 *coordinates =
            nw::lyt::Layout::NewArray<nn::math::VEC2>(4 * capacity);
        vertices->entries = reinterpret_cast<DisplayTexCoords *>(coordinates);
        if (vertices->entries)
            vertices->capacity = capacity;
    }
}
