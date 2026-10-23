/* SPDX-License-Identifier: GPL-3.0-only
 * Continues the measured native image-command candidate; NintendoWare4Ctr
 * commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified: command initialization and width updates use natural static helpers.
 */
#include "game/texture_mapper.h"
using namespace nw::gfx;

static u32 *initialize_texture_command(ResPixelBasedTextureMapper mapper, unsigned words, u32 samplerMask)
{
    u32 *command = mapper.GetCommandCache();
    mapper.SetCommandSizeToSend(words * sizeof(u32));
    command[0] &= ~15u;
    command[3] &= ~0x0ff00000u;
    command[4] &= ~0x07ff07ffu;
    command[5] &= ~(0x30u | samplerMask);
    command[6] &= ~0x000f0000u;
    command[7] &= ~0x0fffffffu;
    return command;
}
static void append_texture_width(u32 *command, ResPixelBasedTexture texture)
{
    command[4] |= (static_cast<u32>(texture.GetWidth()) & 0x7ffu) << 16;
}
extern "C" void func_001c8424(ResPixelBasedTextureMapper mapper, ResImageTexture texture)
{
    u32 *command = initialize_texture_command(mapper, 8, 0);
    ResPixelBasedImage image = texture.GetImage();
    u32 address = image.GetImageAddress();
    u32 format = texture.GetFormatHW();
    command[0] |= format & 0xf;
    command[3] |= 4u << 20;
    command[1] |= 0x000f0000u;
    command[4] |= texture.GetHeight() & 0x7ff;
    append_texture_width(command, texture);
    if (format == ResImageTexture::FORMAT_HW_ETC1)
        command[5] |= 2u << 4;
    ResTextureSampler sampler = mapper.GetSampler();
    if (sampler.GetMinFilter() != ResTextureSampler::MINFILTER_NEAREST &&
        sampler.GetMinFilter() != ResTextureSampler::MINFILTER_LINEAR)
        command[6] |= ((texture.GetMipmapSize() - 1) & 0xf) << 16;
    command[7] |= (func_0018f0ac(reinterpret_cast<const void *>(address)) >> 3) & 0x0fffffff;
}