struct SoundLibraryStatus {
    unsigned char reserved00;
    signed char available;
};
extern struct SoundLibraryStatus data_00242fb8;

int func_00146998(void)
{
    return data_00242fb8.available;
}
