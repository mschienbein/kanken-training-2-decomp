/* Compile-only contract witnesses, not credited original function identities. */
#include "game/applet.h"
#include "game/audio_tasks.h"
#include "game/audio_voice.h"
#include "game/aux_effect.h"
#include "game/display_animation.h"
#include "game/exp_heap.h"
#include "game/input_runtime.h"
#include "game/item_text.h"
#include "game/material.h"
#include "game/service_runtime.h"
#include "game/service_state.h"
#include "game/shared_memory.h"
#include "game/sound_controller.h"

void c008_r2_contract_probe(InputSessionContext *input, int status,
                           ItemPage *page, ItemPageSet *pages,
                           ArchiveIndex *archive, BoundDisplayObject *display,
                           LockedExpHeap *heap, void *allocation,
                           AudioChannelPacket *packet, unsigned char mode,
                           CtrHandle *handle, SharedMemory **memory)
{
    func_001838ec(func_00185de4());
    func_001462d4(input, status);
    func_001462d4(&bss_004b97ac, status);
    func_0014856c(&bss_004b97ac);
    func_00154224(&bss_004b6888);
    func_00154f78(&bss_004b6888, 0, packet);
    MenuItem *item = func_0017a49c(page, 0, 0);
    func_001eb688(item);
    func_001eb694(item, 0);
    func_00177740(item, 0, 0);
    func_001dd4f8(pages, 0);
    func_001f22ac(archive->table);
    func_0017fd68(display->bindings[0]->pane);
    func_0017fda4(display->bindings[0]->pane, 0);
    func_00193a6c(&heap->base, allocation);
    func_00193a6c(&bss_00495ad0.base, allocation);
    func_00151a1c(&bss_004b32c0.master_volume);
    func_0012ff20(0, mode);
    func_001923d4(mode);
    func_00115040(mode);
    func_0012349c(handle, memory);
}
