class Dimensions {
public:
    virtual void reserved0();
    virtual void reserved1();
    virtual int width();
    virtual int height();
};
struct View {
    char reserved0[0x24];
    float horizontalScale, verticalScale;
    char reserved1[0x3c - 0x2c];
    Dimensions *dimensions;
};
extern "C" void func_0010c498(View *view, float x, float y)
{
    float horizontal, vertical;
    horizontal = x / static_cast<float>(view->dimensions->width());
    vertical = y / static_cast<float>(view->dimensions->height());
    view->horizontalScale = horizontal;
    view->verticalScale = vertical;
}
