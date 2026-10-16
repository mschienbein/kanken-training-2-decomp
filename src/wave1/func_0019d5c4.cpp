class Resource {
public:
    virtual unsigned read(unsigned *transferred, unsigned long long offset,
                          void *buffer, unsigned size);
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    virtual void slot10();
    virtual void slot11();
    virtual void release();
};

extern "C" void func_0019d5c4(Resource *resource)
{
    if (resource != 0)
        resource->release();
}
