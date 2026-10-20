class ScaleTextureDimensions {
public:
    virtual void slot00();
    virtual void slot01();
    virtual int width() const;
    virtual int height() const;
};
struct TextureCoordinateScale {
    unsigned char reserved00[0x24];
    float x, y;
    unsigned char reserved2c[0x10];
    ScaleTextureDimensions *texture;
};

extern "C" void func_0010c498(TextureCoordinateScale *scale, float width, float height)
{
    float x = width / scale->texture->width();
    int texture_height = scale->texture->height();
    scale->x = x;
    scale->y = height / texture_height;
}
