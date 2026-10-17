float func_00167000(const float *rectangle, float value)
{
    if (rectangle[1] > value)
        value = rectangle[1];
    else if (rectangle[1] + rectangle[3] < value)
        value = rectangle[1] + rectangle[3];
    return value;
}
