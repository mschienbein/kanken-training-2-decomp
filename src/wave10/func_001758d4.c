/* INTEGRATOR: differs from include/game/notification.h: signed selector bytes at payload offsets 0x34 and 0x35 refine the reserved tail. */
struct NotificationPayload {
    unsigned reserved00[12];
    unsigned char reserved30[2];
    unsigned char flags;
    unsigned char reserved33;
    signed char selection;
    signed char mode;
    unsigned char reserved36[0xb70a];
};

struct SavedNotification {
    int index;
    unsigned reserved;
    struct NotificationPayload payload;
};

extern struct SavedNotification bss_00280af0;

int func_001758d4(void)
{
    return bss_00280af0.payload.selection;
}
