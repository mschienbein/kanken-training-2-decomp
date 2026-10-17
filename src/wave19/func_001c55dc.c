int func_001c55dc(int year)
{
    if (year % 400 == 0)
        return 1;
    if (year % 100 == 0)
        return 0;
    if ((year & 3) == 0)
        return 1;
    return 0;
}
