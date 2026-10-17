#include "game/audio_voice.h"
extern "C" AudioVoice *func_001d89ac(AudioTrackControl *parent, unsigned char index, const AudioVoiceStartArgs *arguments)
{
    AudioVoiceAllocator *allocator = parent->allocator;
    AudioVoice *(*allocate)(AudioVoiceAllocator *, AudioTrackControl *, unsigned char, const AudioVoiceStartArgs *) = allocator->vtable->allocate;
    return allocate(allocator, parent, index, arguments);
}
