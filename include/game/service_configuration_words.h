#ifndef GAME_SERVICE_CONFIGURATION_WORDS_H
#define GAME_SERVICE_CONFIGURATION_WORDS_H

struct ServiceConfigurationWords;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ServiceConfigurationWords {
    unsigned int words[16];
};

#endif
