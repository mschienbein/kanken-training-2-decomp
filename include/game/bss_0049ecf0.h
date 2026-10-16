#ifndef GAME_BSS_0049ECF0_H
#define GAME_BSS_0049ECF0_H

/* Command-queue/notification state at 0x49ecf0 (.bss). Partial: only fields accessed by
 * matched functions are named; offsets are fixed by the original code. */

struct CommandQueue;

struct CommandSystem {
    unsigned char reserved00[0x11];
    unsigned char pending;                  /* 0x011 */
    unsigned char enabled;                  /* 0x012 */
    unsigned char reserved13[0x89];
    struct CommandQueue *writing;           /* 0x09c */
    struct CommandQueue *executing;         /* 0x0a0 */
    unsigned char reserveda4[0xb4];
    int counter;                            /* 0x158 */
    unsigned char reserved15c[4];
    void (*callback)(int event);            /* 0x160 */
};

extern struct CommandSystem bss_0049ecf0;

#endif
