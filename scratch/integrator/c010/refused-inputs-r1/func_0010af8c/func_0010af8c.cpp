#include "game/runtime.h"
#include "game/result.h"
#include "game/synchronization.h"

struct FilesystemWorkItem {
    int active;
    int kind;
    const char *path;
    void *allocation;
    unsigned reserved10[6];
    int reformat;
};
struct FilesystemWorker {
    struct FilesystemWorkItem *items;
    int count;
    int result;
    unsigned reserved0c;
    int pending;
    int stop;
};

/* INTEGRATOR: move to include/game/filesystem_worker.h */
extern "C" {
extern struct FilesystemWorker bss_002d2138;
extern const char *data_00242218;
extern void func_0011247c(void);
extern void func_00112464(void);
extern void func_00112470(void);
extern void func_00112488(void);
extern unsigned func_00120e40(unsigned files, unsigned directories, unsigned char duplicate_data);
extern int func_00110f7c(const char *path, int read_only);
extern int func_00111570(struct FilesystemWorkItem *item);
extern int func_0011122c(struct FilesystemWorkItem *item);
extern unsigned func_001131f8(const char *path);
extern unsigned func_0011343c(const char *path);

void func_0010af8c(struct SignalState *signal);
}

static inline void check_result(unsigned pc, Result result)
{
    switch (result.failed()) {
    case 0: break;
    default: func_0019d6c4(result.value, pc); break;
    }
}

void func_0010af8c(struct SignalState *signal)
{
    for (;;) {
        struct FilesystemWorkItem *item;
        int i;
        func_00195d94(signal);
        if (bss_002d2138.stop)
            return;
        if (!bss_002d2138.pending)
            continue;
        item = bss_002d2138.items;
        func_0011247c();
        func_00112464();
        if (item->reformat) {
            Result result = func_00120e40(1, 1, 1);
            /* [INFERENCE] the sign test before check_result reproduces the
             * original's predicated tst/lsrs sequence. */
            if (result.value & 0x80000000)
                check_result(__current_pc(), result);
        }
        bss_002d2138.result = func_00110f7c(data_00242218, item->kind);
        for (i = 0; i < bss_002d2138.count; ++i, ++item) {
            if (item->active) {
                if (bss_002d2138.result == 0) {
                    if (item->kind == 0)
                        bss_002d2138.result = func_00111570(item);
                    else if (item->kind == 1)
                        bss_002d2138.result = func_0011122c(item);
                }
                if (item->allocation)
                    func_0019dc20(item->allocation);
                item->active = 0;
                item->allocation = 0;
            }
        }
        {
            const char *path = data_00242218;
            if ((func_001131f8(path) & 0x80000000) == 0)
                func_0011343c(path);
        }
        func_00112470();
        func_00112488();
        bss_002d2138.pending = 0;
    }
}
