#include "game/object_pool.h"
void func_0018d074(int *year_out, int *month_out, int *day_out, int days)
{
    int cycles400, cycles100, cycles4, years;
    int within400, within100, within4, within_year;
    int year, month, day;
    days -= 60;
    cycles400 = days / 146097;
    within400 = days % 146097;
    if (within400 < 0) {
        --cycles400;
        within400 += 146097;
    }
    cycles100 = within400 / 36524;
    within100 = within400 % 36524;
    cycles4 = within100 / 1461;
    within4 = within100 % 1461;
    years = within4 / 365;
    within_year = within4 % 365;
    month = (within_year * 5 + 2) / 153;
    year = cycles400 * 400 + cycles100 * 100 + cycles4 * 4 + years + 2000;
    day = within_year - (month * 153 + 2) / 5 + 1;
    if (years == 4 || cycles100 == 4) {
        month = 11;
        day = 29;
        --year;
    }
    if (month <= 9) {
        month += 3;
    } else {
        month -= 9;
        ++year;
    }
    if (year_out)
        *year_out = year;
    if (month_out)
        *month_out = month;
    if (day_out)
        *day_out = day;
}
