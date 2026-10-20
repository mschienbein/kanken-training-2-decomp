#ifndef GAME_SERVICE_STATE_H
#define GAME_SERVICE_STATE_H

struct InputService;
struct LifecycleCallbackTables;
struct PoolLink;
struct Record150c58;
struct Record150cb8;
struct Record150f2c;
struct Record150f6c;
struct S150c58;
struct S150cb8;
struct S150f2c;
struct S150f6c;
struct ServicePollRegistration;
struct ServiceRegistration;
struct SoundFileGroup;
struct StreamMutex;
struct WorkQueue;
struct X07NarrowCommand;

#include <stdbool.h>
#include "ctr/svc.h"
#include "game/pool_list.h"
#include "game/service_callbacks.h"
#include "game/service_runtime.h"
#include "game/shared_memory.h"

/* The activity flags, client handle (+0x10), and interval (+0x18) belong
 * to the same published service state at 0x00242c18. */
struct ServiceClientState {
    bool active;
    bool latched;
    unsigned char reserved02[0x0e];
    unsigned handle;
    unsigned reserved14;
    long long interval;
};
struct ServiceStringCounts {
    int single_count;
    int group_count;
};

struct RequestRecord {
    unsigned kind;
    unsigned flags;
    unsigned short count;
    unsigned short length;
    unsigned short text[0x76c];
    unsigned char flag_ee4;
    unsigned char flag_ee5;
    unsigned char flag_ee6;
    unsigned char auxiliary[0x89];
    int handle;
    unsigned short status;
    unsigned char reservedf76[10];
};

/* Partial state at 0x00242a50. The signed flag is read by func_00195e84;
 * the status word at 0x18 is shared by the service-state predicates. */
struct ServiceState {
    signed char flag_00;
    signed char active;
    unsigned char flag_02;
    unsigned char flag_03;
    unsigned char flag_04;
    unsigned char flag_05;
    unsigned char mode;
    signed char flag_07;
    signed char flag_08;
    signed char flag;
    unsigned char flag_0a;
    unsigned char option;
    unsigned char flag_0c;
    unsigned char flag_0d;
    unsigned char reserved0e[2];
    unsigned value10;
    unsigned applet_id;
    volatile unsigned state;
};

struct AppletStartupState {
    signed char ready;
    unsigned char requested;
    unsigned char reserved02[2];
    unsigned reasons;
    struct SignalState event;
};

struct PendingState {
    unsigned char reserved00;
    unsigned char pending;
    signed char blocking;
    unsigned char graphics_active;
    unsigned char completion;
    unsigned char reserved05[3];
    struct ServiceWaitObject *wait_object;
};

/* The handle-bearing object at 0x002430e0 is not the flag block above. */
struct ServiceContext;

#ifdef __cplusplus
extern "C" {
#endif
extern struct ServiceState data_00242a50;
extern struct ServiceClientState data_00242c18;
extern struct ServiceStringCounts data_002430e4;
extern void func_0010b5cc(void *context);
extern struct ServiceContext data_002430e0;
extern struct PendingState data_00242a28;
extern int func_001927a8(void);
extern int func_00192c10(void);
extern void func_00115040(unsigned char mode);
extern void func_00192458(unsigned value);
extern void func_0019051c(struct RequestRecord *record);
extern unsigned char func_00122af0(void);
extern void func_00122fe8(unsigned char value);
extern void func_00122ff8(void);
extern void func_0012369c(void);
extern int func_0018eed0(void);
extern struct ServiceContext *func_0018fcb4(void);
extern unsigned func_0018fa44(const struct ServiceContext *context, void **memory,
                             unsigned handle);
extern void func_001904e0(void);
extern void func_001904ec(void);
extern void func_00191d20(void);
extern void func_00191dac(unsigned char value);
extern int func_00191dbc(void);
extern unsigned func_00191de8(void);
extern void func_00192400(unsigned option);
extern void func_00192484(unsigned char value);
extern unsigned func_00192794(void);
extern int func_00195e84(void);
extern void func_00185f70(int enabled);
extern void func_0018ef74(void);
#ifdef __cplusplus
}
#endif


struct ServiceRegistration {
    unsigned reserved00;
    struct PoolLink link;
    unsigned event;
    void *context;
};

struct ServicePollRegistration {
    unsigned reserved00;
    ServicePollCallback callback;
    void *context;
};

struct Record150c58 {
    unsigned char reserved0[0x10];
    float value;
    unsigned char reserved1[0x10];
};

struct S150c58 {
    unsigned char reserved0[0x224];
    unsigned count;
    unsigned char reserved1[0x2074 - 0x228];
#ifndef __cplusplus
    struct Record150c58 records[];
#endif
};

struct Record150cb8 {
    unsigned char reserved0[0x0c];
    float value;
    unsigned char reserved1[0x14];
};

struct S150cb8 {
    unsigned char reserved0[0x224];
    unsigned count;
    unsigned char reserved1[0x2074 - 0x228];
#ifndef __cplusplus
    struct Record150cb8 records[];
#endif
};

struct Record150f2c {
    unsigned char reserved0[0x14];
    float value;
    unsigned char reserved1[0x0c];
};

struct S150f2c {
    unsigned char reserved0[0x224];
    unsigned count;
    unsigned char reserved1[0x2074 - 0x228];
#ifndef __cplusplus
    struct Record150f2c records[];
#endif
};

struct Record150f6c {
    unsigned char reserved0;
    unsigned char enabled;
    unsigned char reserved1[0x22];
};

struct S150f6c {
    unsigned char reserved0[0x224];
    unsigned count;
    unsigned char reserved1[0x2074 - 0x228];
#ifndef __cplusplus
    struct Record150f6c records[];
#endif
};

struct LifecycleCallbackTables {
    LifecycleCallback resumed[8];
    LifecycleCallback exiting[8];
};

struct X07NarrowCommand {
    unsigned int header;
    union {
        unsigned int size;
        CtrResult result;
    } body;
    union {
        unsigned short first;
        unsigned char output;
    } first;
    unsigned short padding_first;
    unsigned short second;
    unsigned short padding_second;
    unsigned int descriptor;
    void *buffer;
};

struct WorkCommand {
    unsigned operation, flags, reserved08;
    int first, second;
    unsigned reserved14[3];
};
struct WorkQueue {
    int enabled;
    int current;
    int active;
    unsigned char reserved0c[0x1018];
    int writeIndex;                         /* signed modulo-256 producer arithmetic */
    unsigned readIndex;
#ifndef __cplusplus
    struct WorkCommand commands[];
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
extern void func_0012e638(void);
extern void func_001138ec(struct InputService *service);
extern struct StreamMutex bss_0029f534;
extern struct StreamMutex bss_0029f528;
extern void func_00100024(void);
extern void func_00101670(void);
extern void func_0019edbc(void);
extern void func_001016e4(void);
extern void func_001008ac(void);
extern void func_0010163c(void);
extern void func_001016c4(void);
__declspec(noreturn) extern void func_00100d74(void);
extern struct AppletStartupState data_0023fdd8;
extern struct ServiceRegistration bss_004b1c88;
extern unsigned func_0019ed2c(struct ServiceRegistration *entry, unsigned event);
extern int func_00113ddc(unsigned value);
extern struct ServiceRegistration bss_004b1c74;
extern int func_00122b30(void *context);
extern void func_001233c0(ServicePollCallback callback, void *context);
extern unsigned data_00243030;
extern struct ServicePollRegistration data_00243008;
extern CtrHandle data_00242ff8;
extern struct LifecycleCallbackTables bss_0049eee4;
extern unsigned short data_00243124;
extern void (*bss_004b51e8[16])(void *value);
extern struct WorkQueue bss_004990d0;
extern unsigned func_00191dd8(void);
extern void func_0010c1c8(void);
extern void func_00112464(void);
extern void func_00112470(void);
extern void func_0011247c(void);
extern void func_00112488(void);
extern int func_00106aec(void);
extern void func_00106fac(unsigned char value);
extern void func_0010ba4c(void *context);
extern void func_0010ba9c(void *context);
extern int func_0010d0c0(void);
extern void func_00114f90(void);
extern int func_00114fd0(void);
extern void func_00114ff0(unsigned state);
extern void func_00115084(void);
extern void func_0011515c(unsigned parameter);
extern void func_00115344(unsigned handle);
extern void func_00191df8(unsigned char mode);
extern void func_00191e08(unsigned char enabled);
extern CtrResult func_0012ec54(const CtrHandle *session, const void *payload);
extern unsigned func_001d98bc(const unsigned char *data, unsigned offset,
                                  unsigned *track_mask);
extern void func_00150c58(struct S150c58 *object, unsigned mask, float value);
extern void func_00150cb8(struct S150cb8 *object, unsigned mask, float value);
extern void func_00150f2c(struct S150f2c *object, unsigned mask, float value);
extern void func_00150f6c(struct S150f6c *object, unsigned mask, int enabled);
extern CtrResult func_00112494(CtrHandle handle);
extern CtrResult func_0012349c(CtrHandle *handle, struct SharedMemory **address);
extern CtrResult func_0012fa24(const CtrHandle *session, unsigned int register_address,
                        const void *source, unsigned int size);
extern unsigned func_001234e4(const void *buffer, unsigned size, unsigned value);
extern CtrResult func_0013e770(const CtrHandle *session, void *buffer, unsigned int size,
                       unsigned short first, unsigned short second,
                       unsigned char *output);
extern unsigned func_0017da74(unsigned samples, unsigned format);
extern struct SoundFileGroup *func_0014b60c(struct SoundFileGroup *object);
#ifdef __cplusplus
}
#endif

#endif
