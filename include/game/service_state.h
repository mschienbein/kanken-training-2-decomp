#ifndef GAME_SERVICE_STATE_H
#define GAME_SERVICE_STATE_H

/* Partial state at 0x00242a50. The signed flag is read by func_00195e84;
 * the status word at 0x18 is shared by the service-state predicates. */
struct ServiceState {
    unsigned char flag_00;
    signed char active;
    unsigned char reserved02[4];
    unsigned char mode;
    unsigned char reserved07[2];
    signed char flag;
    unsigned char flag_0a;
    unsigned char option;
    unsigned char flag_0c;
    unsigned char flag_0d;
    unsigned char reserved0e[10];
    volatile unsigned state;
};

struct PendingState {
    unsigned char reserved00;
    unsigned char pending;
    signed char blocking;
};

/* The handle-bearing object at 0x002430e0 is not the flag block above. */
struct ServiceContext;

#ifdef __cplusplus
extern "C" {
#endif
extern struct ServiceState data_00242a50;
extern struct ServiceContext data_002430e0;
extern struct PendingState data_00242a28;
extern int func_001927a8(void);
extern int func_00192c10(void);
extern void func_00115040(unsigned mode);
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
#ifdef __cplusplus
}
#endif

#endif
