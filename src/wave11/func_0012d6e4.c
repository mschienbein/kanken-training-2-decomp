extern void func_00136450(const char *first, const char *second, const char *third,
                          const char *fourth, const char *fifth);

unsigned func_0012d6e4(int selection)
{
    switch (selection) {
    case 0:
        func_00136450("==", "string", "PricingSelection", "PURCHASABLE", "");
        break;
    case 1:
        func_00136450("==", "string", "PricingSelection", "RELEASED", "");
        break;
    default:
        return 0xe0e13884u;
    }
    return 0;
}
