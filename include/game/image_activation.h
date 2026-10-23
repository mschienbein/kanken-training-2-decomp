#ifndef GAME_IMAGE_ACTIVATION_H
#define GAME_IMAGE_ACTIVATION_H

#include "game/screen.h"

/* Shared with the accepted mask selector; fallback remains unsigned. */
struct EffectPresetMasks {
    unsigned packed_masks[3];
    unsigned short fallback;
};
struct W19EffectPresetBlock {
    unsigned char reserved00[0x4c];
    struct EffectPresetMasks masks;
};

#ifdef __cplusplus
extern "C" {
#endif
extern const struct W19EffectPresetBlock data_002181c0;
void func_001494dc(struct W10DrawingInput *state);
void func_0018191c(struct W10DrawingInput *state);
#ifdef __cplusplus
}
#endif

#endif
