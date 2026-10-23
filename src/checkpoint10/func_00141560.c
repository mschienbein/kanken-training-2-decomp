extern void func_00144810(unsigned mode, const float values[4]);

void func_00141560(unsigned mode, float value)
{
    float values[4];
    values[0] = value;
    values[1] = 0.0f;
    values[2] = 0.0f;
    values[3] = 1.0f;
    func_00144810(mode, values);
}
