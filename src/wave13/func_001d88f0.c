#include "game/audio_voice.h"
#include "game/sequence.h"
void func_001d88f0(struct SequencePlayer *player, int event, struct AnimationTrack *track)
{
    if (player->callback) {
        struct SequenceVariables variables;
        struct SequenceTrackState *state;
        variables.player = player->variables;
        variables.global = bss_004b8984;
        state = &track->state;
        variables.track = func_00169f9c(track, 0);
        variables.flag = state->flag;
        player->callback(event, &variables, player->context);
        state->flag = variables.flag;
    }
}
