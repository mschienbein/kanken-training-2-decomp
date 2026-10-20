/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_Voice.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * Modified 2026-10-20: target-version Voice/AudioChannel, RecordState and waveform
 * contracts; byte-sized pan/format storage and one C-linkage callback protocol.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_VOICE_H_
#define NW_SND_VOICE_H_

#include "../../nn/snd.h"
#include "../../nn/snd/CTR/Common/snd_Adpcm.h"
#include "snd_Global.h"
#include "../ut/ut_LinkList.h"
#include "game/voice_wave_info.h"

struct AudioChannel;
struct RecordState;

namespace nw {
namespace snd {
namespace internal {
namespace driver {


class VoiceManager;
class Voice;
extern "C" {
    typedef void VoiceCallbackFunction(Voice*, int, void*);
}

class Voice
{
    friend class VoiceManager;

public:
    static const int UPDATE_START           = 0x0001 << 0;
    static const int UPDATE_PAUSE           = 0x0001 << 1;
    static const int UPDATE_SRC             = 0x0001 << 2;
    static const int UPDATE_MIX             = 0x0001 << 3;
    static const int UPDATE_LPF             = 0x0001 << 4;
    static const int UPDATE_BIQUAD          = 0x0001 << 5;
    static const int UPDATE_VE              = 0x0001 << 6;

    enum VoiceCallbackStatus
    {
        CALLBACK_STATUS_FINISH_WAVE,
        CALLBACK_STATUS_CANCEL,
        CALLBACK_STATUS_DROP_VOICE,
        CALLBACK_STATUS_DROP_DSP
    };

    typedef VoiceCallbackFunction* VoiceCallback;

    static const int CHANNEL_MAX = nw::snd::internal::WAVE_CHANNEL_MAX;

    static const f32 VOLUME_MIN;
    static const f32 VOLUME_DEFAULT;
    static const f32 VOLUME_MAX;
    static const f32 PAN_LEFT;
    static const f32 PAN_CENTER;
    static const f32 PAN_RIGHT;
    static const f32 SPAN_FRONT;
    static const f32 SPAN_CENTER;
    static const f32 SPAN_REAR;
    static const f32 CUTOFF_FREQ_MIN;
    static const f32 CUTOFF_FREQ_MAX;
    static const f32 BIQUAD_VALUE_MIN;
    static const f32 BIQUAD_VALUE_MAX;
    static const f32 SEND_MIN;
    static const f32 SEND_MAX;

private:
    static const int PRIORITY_MAX = nn::snd::CTR::VOICE_PRIORITY_NODROP;
    static const int PRIORITY_MIN = 0;
    static const u32 VOICE_PRIORITY_RELEASE = 1;

public:
    static const int PRIORITY_NODROP = PRIORITY_MAX;
    static const int PRIORITY_RELEASE = 1;

public:
    Voice();
    ~Voice();

    bool Alloc(int channelCount, int priority, Voice::VoiceCallback callback, void* callbackData);
    void Free();
    void Initialize(const ::VoiceWaveInfo& waveInfo, u32 startOffset);

    void Start();
    void Stop();
    void StopFinished();
    void Pause(bool flag);

    void Calc();
    void Update();

    bool IsActive() const { return channels[0] != NULL; }
    bool IsRun() const;
    bool IsPause() const { return flag17 != 0; }
    bool IsPlayFinished() const;

    f32 GetVolume() const { return value24; }
    void SetVolume(f32 volume);

    f32 GetPitch() const { return value28; }
    void SetPitch(f32 pitch);

    void SetPanMode(PanMode panMode);
    void SetPanCurve(PanCurve panCurve);

    f32 GetPan() const { return value30; }
    void SetPan(f32 pan);

    f32 GetSurroundPan() const { return value34; }
    void SetSurroundPan(f32 span);

    f32 GetLpfFreq() const { return value38; }
    void SetLpfFreq(f32 lpfFreq);

    int GetBiquadType() const { return reserved22; }
    f32 GetBiquadValue() const { return value3c; }
    void SetBiquadFilter(int type, f32 value);

    int GetPriority() const { return priority; }
    void SetPriority(int priority);

    void SetFrontBypass(bool isFrontBypass);
    void SetInterpolationType(u8 interpolationType);

    int GetPhysicalVoiceCount() const { return count; }
    int GetChannelCount() const { return count; }
    const ::AudioChannel* GetHardwareChannel(int channelIndex) const;

    void UpdateVoicesPriority();

    f32 GetMainSend() const { return value44; }
    void SetMainSend(f32 send);

    f32 GetFxSend(AuxBus bus) const { return sends[bus]; }
    void SetFxSend(AuxBus bus, f32 send);

    u32 GetCurrentPlayingSample() const;
    nw::snd::SampleFormat GetFormat() const;
private:
    static void SdkVoiceDropCallbackFunc(::AudioChannel* channel, Voice* voice);
    static void SdkVoiceDropCallbackFuncMulti(::AudioChannel* channel, Voice* voice);

    void InitParam(int channelCount, Voice::VoiceCallback callback,void* callbackData);

    void CalcSrc(bool initialUpdate);
    void CalcVe();
    void CalcMix();
    void CalcLpf();
    void CalcBiquadFilter();

    void CalcMixParam(int channelIndex,nw::snd::internal::MixParam* mix);

    void RunAllHardwareChannel();
    void StopAllHardwareChannel();
    void PauseAllHardwareChannel();

    /* Hack to make --gnu work */
public:
    /* Target S01/S02/S14: one 0x60-byte voice, with byte-sized format/pan
     * controls. HardwareChannel and the high-level Voice are different owners. */
    ::AudioChannel* channels[CHANNEL_MAX];
    int count;
    VoiceCallback callback;
    void* context;
    unsigned char active;
    unsigned char flag15, flag16;
    signed char flag17;
    unsigned char flag18, reserved19, allocating, allocation_failed;
    ::RecordState* record;
    u16 dirty;
    u8 reserved22;
    s8 mode23;
    f32 value24, value28;
    u8 option2c, option2d;
    u8 reserved2e[2];
    f32 value30, value34, value38, value3c;
    int priority;
    f32 value44;
    f32 sends[AUX_BUS_NUM];
    u8 format;
    u8 reserved51[3];
    u32 user_word;

public:
    static u32 FrameToByte(u32 sample, nw::snd::SampleFormat format);

    void AppendWaveBuffer(int channelIndex, ::RecordState* buffer, bool lastFlag);
    void SetAdpcmParam(int channelIndex, const ::AdpcmCoefficients& param);

public:
    ut::LinkListNode link;
};

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_VOICE_H_

