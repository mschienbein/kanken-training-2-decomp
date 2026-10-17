/* [INFERENCE] Only the second word is initialized here. Its original
 * semantic name and signedness are unrecovered; func_00113f7c also uses
 * this object's base address. */
struct StartupSetting {
    unsigned char reserved00[4];
    unsigned value;
};

extern struct StartupSetting data_00242c00;

/* Static-storage initializer for data_00242c00.value. */
void func_00212a78(void)
{
    data_00242c00.value = 6;
}
