struct W15fCostOptions {
    unsigned char reserved00[2];
    unsigned char option;
    unsigned char kind;
};

extern int func_001549c8(void);

int func_00154258(const struct W15fCostOptions *options)
{
    int cost = 52600;
    if (options->kind == 0)
        cost = 57100;
    else if (options->kind == 1)
        cost = 55600;
    else if (options->kind == 2) {
        if (func_001549c8() == 0)
            cost = 225600;
        else
            cost = 199600;
    }
    if (options->option == 0)
        cost += 1500;
    else if (options->option == 1)
        cost += 10000;
    return cost;
}
