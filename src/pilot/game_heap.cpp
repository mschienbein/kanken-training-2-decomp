class Heap {
public:
    virtual ~Heap();
    virtual void *allocate(unsigned int size, unsigned int alignment);
};
struct Widget;
/* 24-byte configuration that func_001167f8 copies into the widget at +0x30. */
struct WidgetConfig;
extern "C" Widget *func_001167f8(Widget *storage, Heap *heap, const WidgetConfig *config);
extern "C" Widget *func_0010d604(const WidgetConfig *config, Heap *heap)
{
    Widget *storage = static_cast<Widget *>(heap->allocate(0x48, 0x20));
    if (storage)
        return func_001167f8(storage, heap, config);
    return storage;
}
