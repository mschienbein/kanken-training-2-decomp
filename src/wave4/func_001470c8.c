struct ManagerLock {
    unsigned int state;
};
struct ObjectManager {
    unsigned char reserved00[0x1c4];
    struct ManagerLock lock;
};

extern void func_00195ec4(struct ManagerLock *lock);

void func_001470c8(struct ObjectManager *manager)
{
    func_00195ec4(&manager->lock);
}
