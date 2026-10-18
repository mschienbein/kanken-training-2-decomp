struct CalendarTime {
    long long value;
    CalendarTime() : value(0) {}
    CalendarTime(int year, int month, int day, int hour,
                 int minute, int second, int millisecond);
};
struct TimestampBuffers {
    unsigned char first[0x80];
    unsigned char second[0x100];
    unsigned char third[0x180];
    unsigned char reserved300[0x20];
    CalendarTime timestamp;
    unsigned char reserved328[5];
    unsigned char active;
    TimestampBuffers();
};

/* INTEGRATOR: move to include/game/calendar_time.h */
extern "C" {
void *func_0019df08(void *destination, unsigned int size);
}

TimestampBuffers::TimestampBuffers()
{
    func_0019df08(first, sizeof(first));
    func_0019df08(second, sizeof(second));
    func_0019df08(third, sizeof(third));
    timestamp = CalendarTime(1970, 1, 1, 0, 0, 0, 0);
    active = 0;
}
