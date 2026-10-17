#ifndef GAME_AUDIO_FILTER_H
#define GAME_AUDIO_FILTER_H

struct AudioBiquadCoefficients;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct AudioBiquadCoefficients {
    short values[5];
};

#endif
