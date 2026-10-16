class Dimensions {
public:
    virtual void reserved0();
    virtual void reserved1();
    virtual int width();
    virtual int height();
};
struct Vector2 {
    float x, y;
    Vector2(float horizontal, float vertical) : x(horizontal), y(vertical) {}
};
struct View {
    char reserved0[0x24];
    Vector2 scale;
    char reserved1[0x3c - 0x2c];
    Dimensions *dimensions;
};
extern "C" void func_0010c498(View *view, float x, float y)
{
    view->scale = Vector2(x / static_cast<float>(view->dimensions->width()),
                          y / static_cast<float>(view->dimensions->height()));
}
