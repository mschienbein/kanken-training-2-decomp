struct HwrPoint {
    unsigned char x;
    unsigned char y;
};

struct HwrStroke {
    unsigned short count;
    struct HwrPoint points[];
};

extern struct HwrStroke bss_004b94e8;
extern int func_00156f04(void);
extern int func_00156f50(void);
extern int func_00156f74(struct HwrStroke *stroke, int update_selection);

int func_0015d548(const unsigned char *encoded)
{
    struct HwrStroke *stroke = &bss_004b94e8;
    unsigned short cursor;
    unsigned char maximum;
    if (encoded == 0)
        return -1;
    if (func_00156f04() == -1)
        return -1;
    cursor = 0;
    maximum = 0x7f;
    while (encoded[cursor] != 0xff || encoded[cursor + 1] != 0xff) {
        unsigned short count = 0;
        if (cursor >= 2046)
            goto failure;
        if ((encoded[cursor] & 0x80) != 0) {
            /* ORIGINAL BUG: the continuation loop does not recheck the
             * 2046-byte limit before reading subsequent coordinate pairs. */
            do {
                if (encoded[cursor] == 0xff && encoded[cursor + 1] == 0xff)
                    goto failure;
                stroke->points[count].x = encoded[cursor++] & 0x7f;
                stroke->points[count].y = encoded[cursor++];
                if (stroke->points[count].y >= 0x80)
                    stroke->points[count].y = maximum;
                if (count != 0) {
                    if (stroke->points[count].x == stroke->points[count - 1].x &&
                        stroke->points[count].y == stroke->points[count - 1].y)
                        --count;
                    if (count >= 255)
                        --count;
                }
                ++count;
            } while ((encoded[cursor] & 0x80) != 0);
            if (count != 0) {
                stroke->points[count].x = encoded[cursor++] & 0x7f;
                stroke->points[count].y = encoded[cursor++];
                if (stroke->points[count].y >= 0x80)
                    stroke->points[count].y = maximum;
                stroke->count = count + 1;
                if (func_00156f74(stroke, 0) == -1)
                    goto failure;
                continue;
            }
        }
        cursor += 2;
    }
    return 0;
failure:
    func_00156f50();
    return -1;
}
