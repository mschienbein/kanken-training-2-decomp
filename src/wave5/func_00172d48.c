struct NotificationPayload {
    unsigned words[0xb740 / sizeof(unsigned)];
};
struct SavedNotification {
    int index;
    unsigned reserved;
    struct NotificationPayload payload;
};
extern struct NotificationPayload bss_002476b0[];
extern struct SavedNotification bss_00280af0;
extern void func_0015d6e0(int value);
extern void func_00169b58(void);
extern void func_00192720(void *destination, const void *source, unsigned size);

void func_00172d48(int index)
{
    if (index == -1) {
        index = bss_00280af0.index;
        func_00192720(&bss_002476b0[index], &bss_00280af0.payload, sizeof(struct NotificationPayload));
    }
    func_0015d6e0(index);
    func_00169b58();
}
