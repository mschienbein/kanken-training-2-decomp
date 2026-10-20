#ifndef GAME_APPLICATION_CONFIGURATION_H
#define GAME_APPLICATION_CONFIGURATION_H
#include "game/sdk_init.h"
#include "game/runtime.h"
/* [INFERENCE] Four-byte application configuration block and consumed roles. */
struct ApplicationConfiguration {
    unsigned char reserved00, latched, timeout, reserved03;
};
extern "C" {
Result func_0011375c(unsigned char *state);
void func_001136a4(unsigned char state);
unsigned func_00121764(void *destination, unsigned size, unsigned block);
int func_0010bff4(void);
int func_0010c0a4(void);
}
#endif
