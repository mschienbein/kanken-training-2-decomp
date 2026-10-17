struct CalendarRange {
    unsigned char reserved00[0xb8];
    int year;
    int month;
    int first_year;
    int first_month;
    int last_year;
    int last_month;
};

int func_0016f594(const struct CalendarRange *range, int offset)
{
    int year = range->year;
    int month = range->month + offset;
    if (month > 12) {
        month -= 12;
        ++year;
    } else if (month < 1) {
        month += 12;
        --year;
    }
    if (year < range->first_year || year > range->last_year)
        return 0;
    if (year == range->first_year && month < range->first_month)
        return 0;
    if (year == range->last_year && month > range->last_month)
        return 0;
    return 1;
}
