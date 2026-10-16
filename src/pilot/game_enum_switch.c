extern int func_001795bc(void *object);

int func_001718ec(void *object)
{
    int result;
    /* ORIGINAL BUG: out-of-range selector leaves the result uninitialized. */
    switch (func_001795bc(object)) {
    case 0: result = 1; break;
    case 1: result = 2; break;
    case 2: result = 3; break;
    case 3: result = 4; break;
    case 4: result = 5; break;
    case 5: result = 6; break;
    case 6: result = 7; break;
    case 7: result = 8; break;
    case 8: result = 9; break;
    case 9: result = 10; break;
    case 10: result = 11; break;
    case 11: result = 12; break;
    }
    return result;
}

int func_00171c7c(void *object)
{
    int result;
    /* ORIGINAL BUG: out-of-range selector leaves the result uninitialized. */
    switch (func_001795bc(object)) {
    case 0: result = 2; break;
    case 1: result = 3; break;
    case 2: result = 4; break;
    case 3: result = 5; break;
    case 4: result = 6; break;
    case 5: result = 7; break;
    case 6: result = 8; break;
    case 7: result = 9; break;
    case 8: result = 10; break;
    case 9: result = 11; break;
    case 10: result = 12; break;
    case 11: result = 13; break;
    }
    return result;
}
