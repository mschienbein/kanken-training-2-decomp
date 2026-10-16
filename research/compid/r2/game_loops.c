struct Slot {
    int state;
    char reserved[0x88];
};
struct StartupState {
    struct Slot slots[4];
    float elapsed;
    int enabled;
};
extern struct StartupState bss_004972d8;

void func_00103044(void)
{
    int i;
    bss_004972d8.elapsed = 0.0f;
    bss_004972d8.enabled = 1;
    for (i = 0; i < 4; ++i)
        bss_004972d8.slots[i].state = 3;
}

struct Rectangle { int x, y, width, height; };
struct Rectangles {
    int reserved;
    unsigned char enabled;
    char padding[3];
    int count;
    struct Rectangle rectangles[4];
};
void func_0010b598(struct Rectangles *state)
{
    unsigned int i;
    state->enabled = 0;
    state->count = 0;
    for (i = 0; i < 4; ++i) {
        state->rectangles[i].x = 0;
        state->rectangles[i].y = 0;
        state->rectangles[i].width = 0;
        state->rectangles[i].height = 0;
    }
}

struct Resource { void *allocation; int count; int reserved[2]; };
extern struct Resource bss_0049cb24[8];
extern void func_0019dc20(void *allocation);
void func_00105084(void)
{
    int i;
    for (i = 0; i < 8; ++i) {
        if (bss_0049cb24[i].allocation) {
            func_0019dc20(bss_0049cb24[i].allocation);
            bss_0049cb24[i].allocation = 0;
            bss_0049cb24[i].count = 0;
        }
    }
}
