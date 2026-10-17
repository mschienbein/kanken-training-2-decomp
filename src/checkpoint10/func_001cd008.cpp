/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr contributors at
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Selective canonical-header author candidate; GNU GPL version 3.
 */
#include <preinclude.h>
#include <cstring>
#include <nw/lyt/lyt_ResourceAccessor.h>
#include <nw/lyt/lyt_TextureContainer.h>
/* [INFERENCE] Canonical ResourceAccessor-derived cache layout; cache at 0x1c.
 * The padded prefix is a layout view, not a recovered original class name. */
class TextureCacheAccessor28 : public nw::lyt::ResourceAccessor {
public:
    using nw::lyt::ResourceAccessor::LoadTexture;
    unsigned char reserved04[0x18];
    nw::lyt::TextureContainer cache;
};
/* INTEGRATOR: move canonical texture-cache ABIs to a shared header.
 * [INFERENCE] Explicit output spelling represents the observed hidden aggregate-result ABI. */
extern "C" {
void func_001cc444(nw::lyt::TextureInfo *output, const nw::lyt::TextureContainer *cache, const char *name);
const void *func_001cc39c(nw::lyt::TextureContainer *cache, const char *name, const nw::lyt::TextureInfo *value);
}
extern "C" void func_001cd008(nw::lyt::TextureInfo *output, TextureCacheAccessor28 *accessor, const char *name)
{
    func_001cc444(output, &accessor->cache, name);
    if (!output->IsValid()) {
        const nw::lyt::TextureInfo loaded = accessor->LoadTexture(name);
        std::memcpy(output, &loaded, sizeof(*output));
        if (output->IsValid())
            (void)func_001cc39c(&accessor->cache, name, output);
    }
}
