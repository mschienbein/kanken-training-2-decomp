#ifndef GAME_SERVICE_CLIENTS_H
#define GAME_SERVICE_CLIENTS_H

#include "game/result.h"
#include <string.h>
#include "game/applet.h"
#include "game/service_availability.h"
#include "ctr/ipc.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */




/* Partial handle-bearing object; its unused prefix remains opaque. */
struct ServiceHandleState {
    unsigned char reserved00[4];
    CtrHandle handle;
};
struct HardwareInfo { unsigned flags; unsigned reserved04[47]; };
extern "C" {
extern unsigned char data_00242be0;
extern const char data_0021ac13[];
void func_0010b98c(void);
void func_0019df5c(void);
Result func_00112558(void);
Result func_00113c0c(unsigned *handle, const char *name);
CtrResult func_00113c74(void);
extern unsigned data_0024377c;
extern const char data_0021c5d9[];
extern unsigned char data_002430dc;
extern struct ServiceHandleState data_00242c08;
/* The original checked word is the handle subobject at data_00242c08 + 4,
 * not an independently modeled service object. */
extern CtrHandle data_00242c0c;
unsigned func_00192a7c(void);
void func_0019d6c4(unsigned result, unsigned address);
Result func_00121444(void);
bool func_0010c06c(void);
}
#endif

#endif
