#ifndef GAME_STARTUP_CALLBACK_STATE_H
#define GAME_STARTUP_CALLBACK_STATE_H
struct StartupCallbackOperations;
/* Names-admission's initializer and its registered callback establish the
 * 16-byte prefix. [INFERENCE] Native class/operations names are descriptive. */
struct StartupCallbackState {
    const struct StartupCallbackOperations *operations;
    unsigned first;
    unsigned second;
    unsigned third;

#ifdef __cplusplus
    StartupCallbackState();
    ~StartupCallbackState();
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
extern const struct StartupCallbackOperations data_0021c618;
extern struct StartupCallbackState bss_004b4fa4;
void func_001db960(struct StartupCallbackState *object);
#ifdef __cplusplus
}
#endif
#endif
