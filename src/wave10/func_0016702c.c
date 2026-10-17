float func_0016702c(const float *rectangle, float value)
{
    if (rectangle[0] > value)
        value = rectangle[0];
    else if (rectangle[0] + rectangle[2] < value)
        value = rectangle[0] + rectangle[2];
    return value;
}
