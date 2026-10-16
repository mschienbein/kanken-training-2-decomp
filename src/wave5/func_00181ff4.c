typedef union FloatBits { float value; int bits; } FloatBits;
struct TrigEntry { float sine, cosine, sine_step, cosine_step; };
extern const struct TrigEntry data_0021b100[];
extern float func_00182cc4(float value, float divisor);
extern float fabsf(float value);

static __forceinline int float_bits(float value)
{
    FloatBits view;
    view.value = value;
    return view.bits;
}

float func_00181ff4(float angle)
{
    unsigned index;
    float fraction;
    const struct TrigEntry *entry;
    angle = fabsf(angle);
    if (float_bits(angle) >= 0x47800000)
        angle = func_00182cc4(angle, 65536.0f);
    index = (unsigned short)angle;
    fraction = angle - (float)index;
    index &= 255;
    entry = &data_0021b100[index];
    return entry->cosine + fraction * entry->cosine_step;
}
