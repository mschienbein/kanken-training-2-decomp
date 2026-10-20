typedef void (*UpdateCallback)(void);

struct UpdateState {
    UpdateCallback callback;
    unsigned int frame;
};

extern struct UpdateState bss_00495c94;

void func_00107a88(UpdateCallback callback)
{
    bss_00495c94.callback = callback;
    bss_00495c94.frame = 0;
}
