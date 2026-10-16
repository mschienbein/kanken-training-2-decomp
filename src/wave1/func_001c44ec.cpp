struct DeletingResourceOwner;

extern "C" DeletingResourceOwner *func_001c4500(DeletingResourceOwner *owner, int flags);
extern "C" void func_001852f4(void *allocation);

extern "C" void func_001c44ec(DeletingResourceOwner *owner)
{
    func_001852f4(func_001c4500(owner, 0));
}
