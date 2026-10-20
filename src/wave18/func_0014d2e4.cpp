#include "game/prefetch_stream.h"

/* S08: 32 actual RecordState members at +8, followed by the channel's real
 * ADPCM loop-state array; no separate prefix owner or extent is invented. */
StreamChannel::StreamChannel()
{
}
