struct DisplayObject;
struct AnimationTable;

struct DisplayHandle {
    DisplayObject *object;

    void set_active(int active);
};

struct DisplayState {
    unsigned char reserved00[0x10];
    DisplayHandle *handle;
    unsigned char reserved14[0x3c];
    int active;
    unsigned char reserved54[4];
    int length;
};

struct AnimationContext {
    unsigned int reserved00;
    const AnimationTable *table;
};

extern "C" AnimationContext bss_00246b48;
extern "C" int func_0017adc4(const unsigned short *text);
extern "C" void func_0017a968(DisplayObject *object,
                             const AnimationTable *table, int index, int mode);
extern "C" unsigned char func_0017a9a0(const DisplayObject *object, int index);
extern "C" void func_001826fc(DisplayObject *object, int active);
extern "C" void func_00195a08(DisplayObject *object, int index, int active);
extern "C" void func_0015e948(DisplayObject *object, int active);

inline void DisplayHandle::set_active(int active)
{
    DisplayObject *display = object;
    if (display == 0)
        return;
    switch (func_0017a9a0(display, 0)) {
    case 3:
        func_00195a08(display, 0, active);
        break;
    case 5:
        func_0015e948(display, active);
        break;
    case 6:
        func_001826fc(display, active);
        break;
    }
}

extern "C" void func_00179298(DisplayState *state, int active,
                              const unsigned short *text)
{
    DisplayHandle *handle = state->handle;
    if (handle == 0 || handle->object == 0)
        return;

    if (active) {
        int length;
        state->active = 1;
        length = func_0017adc4(text);
        state->length = length;
        func_0017a968(handle->object, bss_00246b48.table, length, 0);
        handle->set_active(1);
    } else {
        state->active = 0;
        handle->set_active(0);
    }
}
