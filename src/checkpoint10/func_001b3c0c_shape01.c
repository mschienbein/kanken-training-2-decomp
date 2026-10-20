struct SliderState {
    unsigned char reserved00[0x10];
    void *primary;
    unsigned char reserved14[0x40];
    int enabled;
    float minimum;
    float maximum;
    float value;
};
extern void func_0016f288(struct SliderState *slider, float value);

static __forceinline float lower_bound(float value, float minimum)
{
    return value > minimum ? value : minimum;
}
static __forceinline float upper_bound(float value, float maximum)
{
    return value > maximum ? maximum : value;
}
void func_001b3c0c(struct SliderState *slider, int delta)
{
    if (slider->primary && slider->enabled) {
        slider->value += (float)delta;
        slider->value = lower_bound(slider->value, slider->minimum);
        slider->value = upper_bound(slider->value, slider->maximum);
        func_0016f288(slider, slider->value);
    }
}
