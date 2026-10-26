/* [INFERENCE] Native texture resource identity is witnessed by the
 * same-address, matching TexResource::Set implementation and canonical layout. */
#define NW_RELEASE
#include <nw/lyt/lyt_TexResource.h>
#include "game/font_resource.h"
#include "game/heap_manager.h"
static int has_bank_a(unsigned size)
{ return func_00149494(&bss_004952f0.vram, size, 0x80); }
static int has_bank_b(unsigned size)
{ return func_001494b8(&bss_004952f0.vram, size, 0x80); }
static void configure_image(unsigned size, int prefer_vram, nw::lyt::TexResource &reader)
{
    if (prefer_vram) {
        if (has_bank_a(size)) {
            reader.SetImageArea(nw::lyt::MEMAREA_VRAMA);
            return;
        }
        if (has_bank_b(size)) {
            reader.SetImageArea(nw::lyt::MEMAREA_VRAMB);
            return;
        }
    }
    reader.SetImageArea(nw::lyt::MEMAREA_FCRAM);
}

void func_0013e18c(void *data, unsigned size, int prefer_vram)
{
    nw::lyt::TexResource reader;
    if (reader.Set(data, size))
        configure_image(size, prefer_vram, reader);
}
