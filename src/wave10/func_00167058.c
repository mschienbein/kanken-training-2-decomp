struct InputRectangle {
    float x, y, width, height;
};

int func_00167058(const struct InputRectangle *rectangle, float x, float y)
{
    if (rectangle->x > x)
        return 0;
    if (rectangle->x + rectangle->width < x || rectangle->y < y)
        return 0;
    if (rectangle->y - rectangle->height > y)
        return 0;
    return 1;
}
