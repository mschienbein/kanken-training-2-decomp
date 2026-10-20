struct PathCursor {
    const unsigned short *begin;
    const unsigned short *current;
    const unsigned short *next;
};

unsigned func_001c52e8(struct PathCursor *cursor, const unsigned short *path)
{
    if (*path != '/')
        return 0xe0e046c9;
    while (path[1] == '/')
        ++path;
    cursor->begin = path;
    cursor->current = path;
    cursor->next = path + 1;
    while (*cursor->next == '/')
        ++cursor->next;
    return 0;
}
