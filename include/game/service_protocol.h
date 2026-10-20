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
#ifdef __cplusplus
extern "C" {
#endif
extern unsigned func_0019058c(const struct ServiceObject *object);
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
