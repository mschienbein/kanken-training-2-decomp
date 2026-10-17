#include <stdbool.h>

/* [INFERENCE] Current UTF-16 component at +8; unused prefix retains the prior view. */
struct PathComponent {
    const unsigned short *begin;
    unsigned int reserved04;
    const unsigned short *component;
};

static __forceinline bool is_root(const unsigned short *name)
{
    return name[0] == 0 && name[-1] == '/';
}
static __forceinline bool is_dot(const unsigned short *name)
{
    return name[0] == '.' && name[1] == 0;
}
static __forceinline bool is_dotdot(const unsigned short *name)
{
    return name[0] == '.' && name[1] == '.' && name[2] == 0;
}

bool func_001efa28(const struct PathComponent *path)
{
    if (is_root(path->component))
        return true;
    if (is_dot(path->component))
        return true;
    if (is_dotdot(path->component))
        return true;
    return false;
}
