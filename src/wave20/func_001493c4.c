float func_001493c4(float value)
{
    unsigned index = (unsigned)(value * 4.0f);
    float slopes[5] = { 4.0f, -4.0f, -4.0f, 4.0f, 4.0f };
    float intercepts[5] = { 0.0f, 2.0f, 2.0f, -4.0f, 0.0f };
    return slopes[index] * value + intercepts[index];
}
