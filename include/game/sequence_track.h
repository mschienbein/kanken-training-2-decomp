#ifndef GAME_SEQUENCE_TRACK_H
#define GAME_SEQUENCE_TRACK_H

struct SequenceTrackView;
struct SequenceCommandProcessor;



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */

struct SequenceTrackView {
    unsigned char reserved00[0x134];
    SequenceCommandProcessor *processor;
};

extern "C" {

bool func_001f3e8c(SequenceCommandProcessor *processor,
                       SequenceTrackView *track, int dispatch);
}
#endif

#endif
