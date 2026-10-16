class Heap {
public:
    virtual ~Heap();
    virtual void *allocate(unsigned int size, unsigned int alignment);
};
struct Widget;
extern "C" Widget *func_001167f8(Widget *storage, Heap *heap, int parameter);
extern "C" Widget *func_0010d604(int parameter, Heap *heap)
{
    Widget *storage = static_cast<Widget *>(heap->allocate(0x48, 0x20));
    if (storage)
        return func_001167f8(storage, heap, parameter);
    return storage;
}
