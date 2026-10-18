#include <preinclude.h>
#include "nw/snd/snd_Global.h"

/* Native five-byte ADHSR value, initialized at data_0024380a. The canonical
 * game/sound_format.h SoundEnvelope is its matching C-compatible field view.
 * [INFERENCE] The original variable name and translation unit are unknown. */
static const nw::snd::AdshrCurve default_envelope(127, 127, 127, 127, 127);
