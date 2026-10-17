struct IndexedRow {
    unsigned char reserved00[0x20];
    int row;
};

float func_0017a424(const struct IndexedRow *state)
{
    return state->row * 24.0f;
}
