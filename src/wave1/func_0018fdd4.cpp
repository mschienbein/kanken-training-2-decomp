struct Interface;

struct InterfaceVTable {
    unsigned char reserved[0x30];
    void (*release)(Interface *);
};

struct Interface {
    const InterfaceVTable *vtable;
};

struct Result {
    unsigned value;
    Result(unsigned code = 0) : value(code) {}
};

extern "C" Result func_0018fdd4(Interface *object)
{
    Result result;
    if (object != 0) {
        object->vtable->release(object);
        result = Result(0);
    } else {
        result = Result(0xc8804465u);
    }
    return result;
}
