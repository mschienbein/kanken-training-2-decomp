#ifndef GAME_SERVICE_AVAILABILITY_H
#define GAME_SERVICE_AVAILABILITY_H

struct Result;
struct ServiceConfigurationWords;
struct ServiceReferenceState;
struct StreamMutex;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ServiceReferenceState {
    unsigned char initialized;
    unsigned char reserved01[3];
    unsigned references;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct StreamMutex bss_0049ee70;
extern struct ServiceReferenceState data_00242a1c;
extern const char data_0021ab9c[];
int func_0019edcc(void);
void func_0010bed4(void);
void func_0010be58(void);
void func_0010bf74(void);
void func_0010bf60(void);
void func_0010bef8(void);
void func_0010bef4(void);
void func_0012d0a4(void);
int func_00112518(unsigned limit);
extern struct Result func_0010b628(void);
extern struct Result func_00121e80(struct ServiceConfigurationWords *information);
#ifdef __cplusplus
}
#endif

#endif
