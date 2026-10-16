#ifndef GAME_BSS_0049ECF0_H
#define GAME_BSS_0049ECF0_H

/* Command-queue/notification state at 0x49ecf0 (.bss). Partial: only fields accessed by
 * matched functions are named; offsets are fixed by the original code. */

/* One 28-byte queued command. */
struct Command {
    unsigned char kind;                     /* 0x00 */
    unsigned char reserved01;
    unsigned char ready;                    /* 0x02 */
    unsigned char reserved03;
    const void *source;                     /* 0x04 */
    unsigned size;                          /* 0x08 */
    unsigned flags;                         /* 0x0c */
    unsigned char reserved10[12];
};

struct CommandQueue {
    unsigned char reserved00[4];
    unsigned char *base;                    /* 0x04 */
    unsigned char reserved08[4];
    unsigned offset;                        /* 0x0c */
    unsigned char reserved10[4];
    unsigned committed_offset;              /* 0x14 */
    struct Command *commands;               /* 0x18 */
    unsigned char reserved1c[4];
    int count;                              /* 0x20 */
    int completed_count;                    /* 0x24 */
};

struct CommandSystem {
    unsigned char reserved00[0x11];
    unsigned char pending;                  /* 0x011 */
    unsigned char enabled;                  /* 0x012 */
    unsigned char reserved13[5];
    unsigned char ready;                    /* 0x018 */
    unsigned char reserved19[0x83];
    struct CommandQueue *writing;           /* 0x09c */
    struct CommandQueue *executing;         /* 0x0a0 */
    unsigned char reserveda4[0xb4];
    int counter;                            /* 0x158 */
    unsigned char reserved15c[4];
    void (*callback)(int event);            /* 0x160 */
};

extern struct CommandSystem bss_0049ecf0;

#ifdef __cplusplus
extern "C" {
#endif

extern void func_0012fb38(void);              /* lock the command mutex */
extern void func_0012fb48(void);              /* unlock the command mutex */

#ifdef __cplusplus
}
#endif

#endif
