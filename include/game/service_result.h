#ifndef GAME_SERVICE_RESULT_H
#define GAME_SERVICE_RESULT_H
struct ServiceResetState;

struct Result;
struct ServiceContext;
struct ServiceObject;
struct ServiceResult;
struct SignalState;

#include "game/result.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ServiceResult {
    int code;
    unsigned description;
#ifdef __cplusplus
    ServiceResult(struct Result code);
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
extern int func_0012d43c(void);
extern int func_00186538(void);
extern void func_00112c98(struct ServiceResetState *state, unsigned handle);
extern unsigned data_00242ffc;
extern void func_0018221c(struct SignalState *event);
extern int func_0013622c(unsigned handle, unsigned char *value);
extern int func_001266e4(const struct ServiceResult *result);
extern int func_0012672c(const struct ServiceResult *result);
extern void func_00112c80(struct ServiceObject *object);
extern struct Result func_00192ae8(const struct ServiceObject *object);
extern void func_00192ab0(struct ServiceObject *object, struct ServiceResult result);
struct ServiceResult func_00112de0(struct ServiceObject *object, const char *name);
extern struct ServiceResult func_0010b92c(struct ServiceObject *object);
extern void func_001847dc(void);
#ifdef __cplusplus
}
#endif

#endif
