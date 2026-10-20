#ifndef GAME_SOUND_VOICE_H
#define GAME_SOUND_VOICE_H

struct SoundAuxiliaryPlayer;
struct SoundInterpolation;
struct SoundItemPool;
struct SoundPlaybackState;
struct SoundStreamBase;
struct SoundStreamInstance;
struct SoundVoiceBase;
struct SoundVoiceBuffer;
struct SoundVoiceObject;

#include <stdbool.h>
#include "game/runtime.h"
#include "game/sound_runtime.h"
#include "game/prefetch_stream.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
class SoundVoiceBase {
public:
    SoundVoiceBase();
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    unsigned char reserved04[0x40];
};
#endif

#ifdef __cplusplus
class SoundVoiceBuffer {
public:
    SoundVoiceBuffer() : buffer(0), size(0) {}
    virtual void bufferSlot();
    void *buffer;
    unsigned size;
};
#endif


#ifdef __cplusplus
class SoundVoiceObject : public SoundVoiceBase, public SoundVoiceBuffer {
public:
    SoundVoiceObject();
    virtual void slot00();
    unsigned char reserved50[0x34];
    bool active;
    unsigned char reserved85[0x87];
    void *field10c;
    unsigned char reserved110[0x110];
    unsigned field220;
    unsigned field224;
    StreamChannel elements[8];
};
#endif

#ifdef __cplusplus
struct SoundStreamBase : SoundPlaybackState {
    SoundStreamBase() { vtable = data_002245ec; }
};
#endif

#ifdef __cplusplus
struct SoundAuxiliaryPlayer {
    unsigned char reserved00[0xb94];
    SoundAuxiliaryPlayer();
};
#endif

#ifdef __cplusplus
struct SoundInterpolation {
    float value, target;
    unsigned remaining, duration;
    SoundInterpolation();
};
#endif

#ifdef __cplusplus
struct SoundStreamInstance : SoundStreamBase {
    SoundVoiceObject voice;
    unsigned char reserved2180[0x194];
    SoundAuxiliaryPlayer auxiliary;
    unsigned char reserved2ea8[0x19c];
    SoundItemPool *owner;
    SoundInterpolation interpolation[4];
    unsigned char reserved3088[0x206];
    bool active;
    unsigned char reserved328f;
    unsigned field3290, field3294;
    SoundStreamInstance(SoundItemPool *pool);
};
#endif

#endif
