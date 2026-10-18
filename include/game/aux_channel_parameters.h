#ifndef GAME_AUX_CHANNEL_PARAMETERS_H
#define GAME_AUX_CHANNEL_PARAMETERS_H
#include "game/audio_dsp.h"
#include "game/service_runtime.h"

/* Canonical DSP parameter objects are selected by the serial bank. The
 * enabled/count fields refine their existing reserved span without a pointer
 * cast or a second ChannelParameters owner. Roles remain [INFERENCE]. */
#ifdef __cplusplus
extern "C" {
#endif
int func_00155a44(struct ServiceCoordinator *coordinator, int channel);
int func_00155ab4(struct ServiceCoordinator *coordinator, int channel, short count);
#ifdef __cplusplus
}
#endif
#endif
