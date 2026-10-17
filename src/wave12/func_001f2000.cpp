#include "game/sound_controller.h"
/* Partial nw::io::FileStream interface in public NintendoWare4Ctr slot order.
 * [INFERENCE] This forwarding object's concrete subclass is not recovered. */


extern "C" unsigned func_001f2000(const ForwardingFileStream *object)
{
    return object->stream->GetSize();
}
