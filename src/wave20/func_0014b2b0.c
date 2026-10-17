struct TrigEntry { float sine, cosine, sine_step, cosine_step; };
extern const struct TrigEntry data_0021b100[];

float func_0014b2b0(unsigned angle)
{
    float fraction = (float)(angle & 255u) * (1.0f / 256.0f);
    const struct TrigEntry *entry = &data_0021b100[angle >> 8];
    return entry->sine + entry->sine_step * fraction;
}
