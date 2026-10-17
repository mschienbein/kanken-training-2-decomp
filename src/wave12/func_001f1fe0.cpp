#include "game/sound_controller.h"
/* Partial nw::io::FileStream interface in public NintendoWare4Ctr slot order.
 * [INFERENCE] This forwarding object's concrete subclass is not recovered. */


extern "C" bool func_001f1fe0(const ForwardingFileStream *object)
{
    return object->stream->CanRead();
}
