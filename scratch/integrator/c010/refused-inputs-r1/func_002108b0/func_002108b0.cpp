struct StartupCallbackOperations;

/* [INFERENCE] The descriptive class and field names are unrecovered. The
 * startup code proves a 16-byte object with one operations pointer and three
 * zero words; its destructor is registered for this same object. */
struct StartupCallbackState {
    const StartupCallbackOperations *operations;
    unsigned first;
    unsigned second;
    unsigned third;

    StartupCallbackState();
    ~StartupCallbackState();
};

/* INTEGRATOR: move to include/game/startup_callback_state.h */
extern "C" {
extern const StartupCallbackOperations data_0021c618;
extern StartupCallbackState bss_004b4fa4;
}

inline StartupCallbackState::StartupCallbackState()
    : operations(&data_0021c618), first(0), second(0), third(0)
{
}

StartupCallbackState bss_004b4fa4;
