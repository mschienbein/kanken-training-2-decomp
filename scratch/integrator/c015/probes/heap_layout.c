#include <stddef.h>
#include "game/heap_manager.h"
typedef char VramExtent[(sizeof(struct VramAllocator) == 0x630) ? 1 : -1];
typedef char VramOffset[(offsetof(struct HeapManager, vram) == 4) ? 1 : -1];
typedef char PrimaryOffset[(offsetof(struct HeapManager, primary) == 0x640) ? 1 : -1];
