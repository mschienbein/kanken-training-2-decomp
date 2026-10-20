struct W20FloatProperty {
    unsigned char reserved00[0x20];
    unsigned short dirty;
    unsigned char reserved22[0x22];
    float value;
};

void func_00150054(struct W20FloatProperty *property, float value)
{
    value += 1.0f;
    if (value < 0.0f)
        value = 0.0f;
    if (property->value != value) {
        property->value = value;
        property->dirty |= 8;
    }
}
