#include <preinclude.h>
#include <nw/lyt/lyt_Layout.h>
#include "game/screen.h"

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
        void *storage = nw::lyt::Layout::AllocMemory(sizeof(DisplayTexCoords) * capacity);
        /* No parentheses: default initialization leaves the real quad's floats
         * intentionally uninitialized, unlike Layout::NewArray<T>'s T(). */
        DisplayTexCoords *coordinates = storage ? new (storage) DisplayTexCoords[capacity] : 0;
        vertices->entries = coordinates;
        if (vertices->entries)
            vertices->capacity = capacity;
    }
}
