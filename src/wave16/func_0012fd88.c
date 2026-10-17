struct PathSelection {
    unsigned char enabled;
    unsigned char reserved01[3];
    const char *fallback;
    const char *selected;
};
extern struct PathSelection data_00243024;

void func_0012fd88(const char *path)
{
    if (!data_00243024.selected)
        data_00243024.selected = path;
}
