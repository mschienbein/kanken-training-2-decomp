struct LinearValue {
    float initial, target;
    int duration, elapsed;
};
static __forceinline float interpolate(float initial, float target, int elapsed, int duration)
{
    return initial + (float)elapsed * (target - initial) / (float)duration;
}
float func_00182aa8(const struct LinearValue *value)
{
    if (value->elapsed >= value->duration) return value->target;
    return interpolate(value->initial, value->target, value->elapsed, value->duration);
}
