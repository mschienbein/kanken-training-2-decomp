#include <stddef.h>
#include "game/pool_list.h"
#include "game/runtime.h"
#include "game/font_resource.h"



extern "C" void func_001ccfcc(FontManager *manager)
{
    manager->vtable = data_00224128;
    PoolList *textures = func_001cc4cc(&manager->textures);
    manager = reinterpret_cast<FontManager *>(reinterpret_cast<char *>(textures)
        - offsetof(FontManager, textures));
    PoolList *fonts = func_001cc12c(&manager->fonts);
    manager = reinterpret_cast<FontManager *>(reinterpret_cast<char *>(fonts)
        - offsetof(FontManager, fonts));
    /* [INFERENCE] callee identity: inlined by armlink. The empty base
     * finalizer preserves the object pointer for allocation release. */
    func_001852f4(func_001ef884(manager));
}
