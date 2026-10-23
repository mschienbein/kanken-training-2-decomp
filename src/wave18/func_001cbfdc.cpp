#include <preinclude.h>
#include <nw/lyt/lyt_Util.h>
#include "game/font_resource.h"



namespace nw { namespace lyt {
void DeleteTexture(const TextureInfo &information)
{
    unsigned value = information.GetTextureObject();
    if (data_00242f8c != 0) {
        unsigned texture = value;
        if (texture != 0)
            func_0018f0dc(1, &texture);
    } else if (value != 0) {
        ReleaseCallback release = 0;
        func_00215e88(0, &release);
        release((value & 3u) << 16, 0x101, 0,
                reinterpret_cast<void *>(value & ~3u));
    }
}
} }
