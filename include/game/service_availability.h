#ifndef GAME_SERVICE_AVAILABILITY_H
#define GAME_SERVICE_AVAILABILITY_H

struct Result;
struct ServiceConfigurationWords;
struct ServiceReferenceState;
struct StreamMutex;

#include "game/runtime.h"
#include "ctr/ipc.h"

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
int func_00112518(unsigned limit);
extern struct Result func_0010b628(void);
extern struct Result func_00121e80(struct ServiceConfigurationWords *information);
extern struct StreamMutex bss_004b4fb4;
extern int data_00243120;
extern const char data_0021c61c[];
extern struct Result func_0012ef30(void);
extern unsigned func_0010c33c(void);
extern void func_0010c338(void);
extern unsigned func_00121ddc(unsigned value);
extern unsigned func_00121da0(unsigned value);
/* [INFERENCE] Preserve the flat forwarding ABI, including the separately
 * forwarded final buffer word; no synthetic request-object interface. */
extern struct Result func_00136f38(
    unsigned input1, unsigned input2, const void *input3,
    unsigned input4, const void *input5, unsigned input6,
    void *input7, unsigned input8, void *input9,
    unsigned input10, const void *input11, unsigned input12,
    void *input13, unsigned input14, void *input15,
    unsigned input16, unsigned *input17, const void *input18,
    void *input19, unsigned input20);
extern struct Result func_0012f050(
    unsigned argument1, unsigned argument2, const void *argument3,
    unsigned argument4, const void *argument5, unsigned argument6,
    void *argument7, unsigned argument8, void *argument9,
    unsigned argument10, const void *argument11, unsigned argument12,
    void *argument13, unsigned argument14, void *argument15,
    unsigned argument16, unsigned *argument17, void *argument18,
    unsigned argument19);
#ifdef __cplusplus
}
#endif

#endif
