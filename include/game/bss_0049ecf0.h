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

/* This registry resource is not the unrelated stream RenderResource. */
struct CommandResource {
    unsigned id;
    void *memory;
    unsigned format;
    int width;
    int height;
    unsigned module;
    struct CommandResource *next;
};

struct CommandPatch {
    unsigned reserved00[2];
    unsigned pair_count;
    unsigned reserved0c[3];
};

struct CommandQueue {
    unsigned id;
    unsigned char *base;
    unsigned capacity;
    unsigned offset;
    unsigned committed_bytes;               /* 0x10 */
    unsigned committed_offset;
    struct Command *commands;
    unsigned command_bytes;
    int count;
    int completed_count;
    int cursor;                             /* 0x28 */
    int option;                             /* 0x2c; queried as an unsigned word */
    unsigned captured_size;
    struct CommandPatch *patch;
    void (*callback)(void);                  /* 0x38; no distinct argument prepared */
    unsigned char complete, flush;
    unsigned char reserved3e[2];
    struct CommandQueue *next;
};

struct CommandSystem {
    unsigned char reserved00[8];
    int display_heights[2];
    unsigned char initialized;              /* 0x10 */
    unsigned char pending;
    unsigned char enabled;
    unsigned char reserved13[5];
    unsigned char ready;
    unsigned char reserved19[3];
    struct CommandQueue *queues[32];         /* 0x1c; also walked by shutdown */
    struct CommandQueue *writing;
    struct CommandQueue *executing;
    struct CommandResource *resources[32];   /* 0xa4 */
    int resource_index;
    struct CommandResource *selected[3];
    unsigned values[3];
    unsigned modes[3];
    unsigned char reserved14c[8];
    int begin_counter;                      /* 0x154 */
    int counter;
    void (*begin_callback)(int event);
    void (*callback)(int event);
    int display_mode;
};

extern struct CommandSystem bss_0049ecf0;

#ifdef __cplusplus
extern "C" {
#endif

extern void func_0012fb38(void);              /* lock the command mutex */
extern void func_0012fb48(void);              /* unlock the command mutex */
extern void func_0012ccd8(struct CommandQueue *queue, struct Command *command);

#ifdef __cplusplus
}
#endif

#endif
