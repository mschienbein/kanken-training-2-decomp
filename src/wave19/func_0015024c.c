struct W19FloatProperty {
    unsigned char reserved00[0x20];
    unsigned short flags;
    unsigned short reserved22;
    float value;
};

void func_0015024c(struct W19FloatProperty *property, float value)
{
    if (value < 0.0f)
        value = 0.0f;
    if (property->value != value) {
        property->value = value;
        property->flags |= 0x40;
    }
}
