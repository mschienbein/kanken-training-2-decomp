struct W23JSoundState;
struct W23JSoundVoice;
struct W23JVoiceCallback;

/* INTEGRATOR: move to include/game/sound_voice.h */
extern "C" {
W23JSoundVoice *func_001d4ea8(W23JSoundState *state, int key, int velocity,
                             int value, int enabled);
W23JSoundVoice *func_001f3e50(W23JVoiceCallback *callback,
                             W23JSoundState *state, int key, int velocity,
                             int value, int enabled);
}

/* [INFERENCE] The first argument is the unused receiver of the callback
 * referenced by the sound vtable; the remaining five arguments are forwarded. */
W23JSoundVoice *func_001f3e50(W23JVoiceCallback *, W23JSoundState *state,
                             int key, int velocity, int value, int enabled)
{
    return func_001d4ea8(state, key, velocity, value, enabled);
}
