/* SPDX-License-Identifier: GPL-3.0-only
 * Texture setter and priority-list resort adapted from NintendoWare4Ctr
 * contributors, pinned commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd:
 * sources/libraries/lyt/lyt_TexResource.cpp and libraries/snd/snd_SoundPlayer.cpp.
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * GNU GPL version 3; see the repository LICENSE.txt.
 * [INFERENCE] Address adjacency is a TU hypothesis, not proof that the texture
 * and sound routines historically belonged to the same source file.
 */
#define NW_RELEASE
#include <nw/lyt/lyt_TexResource.h>
#include "game/font_resource.h"
#include "game/sound_runtime.h"

/* INTEGRATOR: move the sound-priority declarations to include/game/sound_runtime.h. */
extern "C" {
void func_00143350(struct SoundFileGroup *player, struct SoundInstance *sound);
void func_00143328(struct SoundFileGroup *player, struct SoundInstance *sound);
}

namespace {
enum {
    TextureAlignment = 128,
    TextureBlockAlignment = 4
};

static bool is_aligned(unsigned value, unsigned alignment)
{
    return (value & (alignment - 1)) == 0;
}

static void *offset_pointer(void *base, unsigned offset)
{
    return static_cast<unsigned char *>(base) + offset;
}

static void erase_priority(PoolList &list, PoolLink &node)
{
    func_0018d868(&list, &node);
}

}

namespace nw { namespace lyt {
bool TexResource::Set(void *data, u32 size)
{
    do {
        if (data == NULL)
            break;
        if (!is_aligned(reinterpret_cast<unsigned>(data), TextureAlignment))
            break;
        if (!is_aligned(size, TextureBlockAlignment))
            break;
        if (size <= sizeof(ut::BinaryFileHeader) + sizeof(res::Image) + sizeof(res::ImageSize))
            break;
        res::ImageSize *image_size = static_cast<res::ImageSize *>(offset_pointer(data, size - sizeof(res::ImageSize)));
        unsigned image_bytes = image_size->imageSize;
        if (image_bytes >= size)
            break;
        /* ORIGINAL BUG: only the stored image offset is checked against size;
         * the aligned header and its block extents are not bounded here. */
        ut::BinaryFileHeader *file = static_cast<ut::BinaryFileHeader *>(offset_pointer(data, ut::RoundUp(image_bytes, TextureBlockAlignment)));
        /* The existing C and native binary-header declarations describe the same
         * binary file layout; keep the established validator contract. */
        if (!func_001813a0(reinterpret_cast<const ::BinaryFileHeader *>(file), res::FILESIGNATURE_CLIM, res::BinaryFileFormatVersion, 1))
            break;
        res::Image *image = NULL;
        ut::BinaryBlockHeader *block = NULL;
        for (int index = 0; index < file->dataBlocks; ++index) {
            block = ut::GetNextBinaryBlockHeader(file, block);
            /* ORIGINAL BUG: a malformed block extent can return a null iterator;
             * the release setter nevertheless reads its signature. */
            switch (block->kind) {
            case res::DATABLOCKKIND_IMAGE:
                image = reinterpret_cast<res::Image *>(block);
                break;
            }
        }
        if (image == NULL)
            break;
        m_pTop = data;
        m_pImage = image;
        m_pImageSize = image_size;
        return true;
    } while (false);
    m_pTop = NULL;
    m_pImage = NULL;
    m_pImageSize = NULL;
    return false;
}
} }

void func_00143328(SoundFileGroup *player, SoundInstance *sound)
{
    erase_priority(player->pending, sound->player_link);
    func_00143350(player, sound);
}
