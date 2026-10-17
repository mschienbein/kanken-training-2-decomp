struct NotificationKeyEntry {
    unsigned char enabled;
    unsigned char reserved01[3];
    unsigned int key;
};
struct NotificationKeyTable {
    unsigned char reserved00[0x62a0];
    struct NotificationKeyEntry entries[180];
};
/* INTEGRATOR: differs from include/game/notification.h: this adds the keyed
 * table within SavedNotification.payload.storage.fields.reserved61d8. */
extern struct NotificationKeyTable bss_00280af0;
extern unsigned int func_00183070(int year, int month, int day);

unsigned char func_001dc100(int year, int month, int day)
{
    unsigned char enabled = 0;
    unsigned int key = func_00183070(year, month, day);
    int index;
    for (index = 0; index < 180; ++index) {
        if (bss_00280af0.entries[index].key == key) {
            enabled = bss_00280af0.entries[index].enabled;
            break;
        }
    }
    return enabled;
}
