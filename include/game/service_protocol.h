#ifndef GAME_SERVICE_PROTOCOL_H
#define GAME_SERVICE_PROTOCOL_H

struct ServiceConfigurationWords;

#include "ctr/svc.h"
#include "game/service_state.h"
struct ServiceObject { unsigned handle; };
struct ServiceResetState { unsigned generation; };
struct ServiceEntry { unsigned value; unsigned char kind, variant; unsigned short attributes; };
struct ServiceList {
    struct ServiceEntry *entries;
    unsigned capacity, count, remainder[36];
};
struct ServiceConfiguration {
    unsigned reserved00[3];
    unsigned value;
    unsigned char kind, variant, mode, reserved13;
    char label[0x40], metadata[0x44];
    unsigned char changed;
};
struct ServiceReport;

/* [INFERENCE] Duration name; the original ABI carries a signed 64-bit value. */
struct AppletTimeout {
    long long nanoseconds;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct AppletTimeout data_00242a40;
extern unsigned data_00242a34;
/* Full event result, including -1. The fifth pointer is optional; the timeout
 * occupies the next aligned pair of stack words, not a RequestWait object. */
extern int func_00191e18(unsigned *handle, void *buffer, unsigned capacity,
    void **memory, unsigned *size, struct AppletTimeout timeout);
extern unsigned func_0019058c(const struct ServiceObject *object);
extern void func_001217b4(void);
extern void func_001224a4(void);
extern void func_0012f51c(void);
extern unsigned char func_0013685c(void);
extern unsigned func_00137770(void *buffer, unsigned size);
extern CtrResult func_0012e7b4(CtrHandle *handle);
extern CtrResult func_001380c8(unsigned value0, unsigned value1,
    unsigned value2, unsigned value3, unsigned char *out);
extern CtrResult func_001383ac(unsigned char identifier);
extern CtrResult func_00138334(unsigned char identifier);
extern CtrResult func_0013e734(unsigned char *region);
extern CtrResult func_001863fc(const CtrHandle *session);
extern unsigned func_00136510(const struct ServiceConfiguration *configuration);
extern unsigned func_001365e0(const struct ServiceConfiguration *configuration);
extern unsigned func_001375b0(unsigned char *out, const struct ServiceReport *report, const char *text);
extern unsigned func_0013eab4(unsigned char *out, const struct ServiceReport *report, const char *text, float seconds);
extern unsigned func_001426f4(const char *text);
/* The mapped-buffer permission is read-only; caller text remains immutable. */
extern int func_001426a8(const void *text, unsigned size);
extern int func_00142718(const void *text, unsigned size);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern CtrResult func_0012f170(struct ServiceConfigurationWords *out);
#ifdef __cplusplus
}
#endif

#endif
