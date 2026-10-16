struct ObjectManager;

extern volatile unsigned int data_00243738;
extern struct ObjectManager bss_004b8fa8;
extern int func_00191c80(volatile unsigned int *guard);
extern struct ObjectManager *func_001807c0(struct ObjectManager *manager);
extern void func_00147260(void);
extern void func_00100000(void);
extern void func_00215ec4(volatile unsigned int *guard);
extern void func_00216140(struct ObjectManager *object, void (*destroy)(void), void (*image_base)(void));

struct ObjectManager *func_00185d1c(void)
{
    if (!(data_00243738 & 1) && func_00191c80(&data_00243738)) {
        struct ObjectManager *manager = func_001807c0(&bss_004b8fa8);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00216140(manager, func_00147260, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_00243738);
    }
    return &bss_004b8fa8;
}
