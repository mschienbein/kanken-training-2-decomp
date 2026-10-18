#include <preinclude.h>

struct TextureBufferOwnerVTable;
/* [INFERENCE] Handle-owner class identity; the three handle fields and
 * destruction order are recovered from the body and calls. */
struct TextureBufferOwner {
    const TextureBufferOwnerVTable *vtable;
    unsigned reserved04;
    GLuint first_texture;
    GLuint second_texture;
    GLuint buffer;
    ~TextureBufferOwner();
};
/* INTEGRATOR: move to include/game/texture_buffer.h. */
extern "C" {
extern const TextureBufferOwnerVTable data_00223ecc;
void func_0016cbf4(GLsizei count, const GLuint *textures);
void func_00215cb0(GLsizei count, const GLuint *buffers);
}

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
