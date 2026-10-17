/* The caller obtains the three calendar fields from func_001dc580. */
struct W11jCalendar {
    unsigned char reserved00[0x0c];
    int year;
    int month;
    int day;
};

void func_001c2280(struct W11jCalendar *calendar, int year, int month, int day)
{
    calendar->year = year;
    calendar->month = month;
    calendar->day = day;
}
