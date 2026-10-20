#include <preinclude.h>

#include "game/render_resource.h"

TextureBufferOwner::~TextureBufferOwner()
{
    GLuint second = second_texture;
    vtable = &data_00223ecc;
    if (second)
        func_0016cbf4(1, &second_texture);
    if (first_texture)
        func_0016cbf4(1, &first_texture);
    if (buffer) {
        GLuint buffer_handle = buffer;
        func_00215cb0(1, &buffer_handle);
    }
}
