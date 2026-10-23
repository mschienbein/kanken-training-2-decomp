struct SoundGlobalDefaults {
    unsigned first, second, third;
};
/* INTEGRATOR: move to include/game/sound_state.h */
extern "C" {
extern SoundGlobalDefaults bss_004b32b4;
}
struct SoundManagerSettings {
    unsigned maximum, interval, reserve;
    float blend, decay;
    SoundGlobalDefaults *defaults;
    float spread, damping;
    unsigned char active, pending;
    unsigned char reserved22[2];
    SoundManagerSettings()
        : maximum(60), interval(4000), reserve(100), blend(0.5f), decay(0.4f),
          defaults(&bss_004b32b4), spread(0.6f), damping(0.4f), active(0), pending(0)
    {}
};
struct SoundStatePair {
    unsigned first, second;
};
struct SoundManagerVTable;
struct SoundManagerState {
    const SoundManagerVTable *vtable;
    unsigned field04, field08;
    SoundManagerSettings settings;
    unsigned field30, reserved34;
    SoundGlobalDefaults defaults_copy;
    unsigned first[4], second[4];
    SoundStatePair pairs[4];
    unsigned third[4], fourth[4];
    unsigned limit;
    SoundStatePair ramps[2];
    unsigned fieldb8, fieldbc, fieldc0, fieldc4;
    SoundStatePair extra_ramp;
    unsigned fieldd0, fieldd4;
    unsigned fifth[4];
    unsigned fielde8, fieldec, fieldf0, fieldf4;
    unsigned reservedf8[2];
    unsigned table_sizes[3];
    unsigned char reserved10c, channels, pending, reserved10f;
    SoundManagerState();
};
struct SoundActorVTable;
struct SoundActorState {
    const SoundActorVTable *vtable;
    unsigned field04, field08;
    unsigned priority;
    float decay, blend;
    unsigned char active;
    unsigned char reserved19[3];
    unsigned field1c, field20;
    void *first[4];
    void *second[4];
    unsigned reserved44;
    unsigned field48, field4c, capacity, field54;
    unsigned char reserved58[5];
    unsigned char channels;
    unsigned char pending;
    unsigned char reserved5f;
    SoundActorState();
};
struct SoundActorSettings {
    unsigned interval;
    float decay, blend;
    unsigned char active;
    SoundActorSettings() : interval(250), decay(0.4f), blend(0.5f), active(0) {}
};
struct SoundSystemState {
    unsigned reserved00[2];
    SoundManagerState manager;
    SoundManagerSettings settings;
    unsigned char reserved13c[0x1b824];
    SoundActorState actor;
    SoundActorSettings actor_settings;
    SoundSystemState();
};

SoundSystemState::SoundSystemState()
{
}
