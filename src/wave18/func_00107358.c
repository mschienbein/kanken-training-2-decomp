extern void func_00195f7c(void);
extern void func_0019b574(unsigned parameter, unsigned *value);
extern void func_0011db78(int count);

struct CommandCapture {
    unsigned reserved00;
    unsigned count;
    unsigned reserved08[6];
};
struct CommandCaptureSystem {
    unsigned reserved00;
    struct CommandCapture *captures;
};
extern struct CommandCaptureSystem bss_002d4030;

static __forceinline void recordCount(struct CommandCapture *capture, unsigned count)
{
    capture->count = count;
}

void func_00107358(int index)
{
    unsigned count;
    func_00195f7c();
    count = 0;
    func_0019b574(0x203, &count);
    if (!count) {
        func_00195f7c();
        func_0019b574(0x203, &count);
    }
    func_0011db78(count);
    recordCount(&bss_002d4030.captures[index], count);
}
