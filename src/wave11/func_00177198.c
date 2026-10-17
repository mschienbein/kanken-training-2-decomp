struct W11TextEntry {
    int text_offset;
    unsigned reserved04;
    int attributes_offset;
    unsigned reserved0c;
};

struct W11TextResource {
    unsigned char reserved00[0x10];
    const struct W11TextEntry *entries;
};

struct W11TextAttributes;
struct W11TextDisplay;

extern const unsigned short *func_00170c00(const struct W11TextResource *resource, int index);
extern const struct W11TextAttributes *func_001dcbd4(const struct W11TextResource *resource, int index);
extern void func_0017a43c(struct W11TextDisplay *display, const unsigned short *text,
                         const struct W11TextAttributes *attributes, int index);
extern const unsigned short data_002276bc[];

void func_00177198(struct W11TextDisplay *display, const struct W11TextResource *resource, int index)
{
    if (index == -1) {
        func_0017a43c(display, data_002276bc, 0, 0);
    } else {
        const struct W11TextAttributes *attributes = func_001dcbd4(resource, index);
        const unsigned short *text = func_00170c00(resource, index);
        func_0017a43c(display, text, attributes, 0);
    }
}
