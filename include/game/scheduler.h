#ifndef GAME_SCHEDULER_H
#define GAME_SCHEDULER_H
struct ScheduledTask {
    unsigned short enabled;
    short previous, next;
    unsigned short priority;
    void (*callback)(void);
    int wait;
    int repeat;
    int reserved14;
};
struct Scheduler {
    struct ScheduledTask tasks[64];
    int current;
    int next;
    short end, begin;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct Scheduler bss_00495c8c;
extern int func_0019b54c(int task);
extern int func_0019b564(void);
extern void func_00195ad0(unsigned int index);
extern void func_001071cc(void);
extern void func_001074d0(void);
extern void func_00107dbc(void);
extern void func_001071ec(void);
extern void func_00110aa4(void);
extern void func_00195168(void);
extern void func_0012d1cc(void);
extern void func_0012d1d8(int count, int preserve);
extern void func_0012ced8(void);
extern void func_001951dc(unsigned int handle);
extern void func_0012d1c8(void);
extern int func_00141c70(void);
#ifdef __cplusplus
}
#endif
#endif
