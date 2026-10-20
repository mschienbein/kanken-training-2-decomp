#ifndef GAME_FONT_H
#define GAME_FONT_H

struct FontInformation;
struct FontResource;
struct FontVTable;

struct FontFile {
    unsigned signature;
    unsigned short byteOrder, headerSize;
    unsigned version, fileSize;
    unsigned short blockCount, reserved12;
#ifndef __cplusplus
    unsigned char blocks[];
#endif
};

struct FontBlock {
    unsigned signature, size;
#ifndef __cplusplus
    unsigned char data[];
#endif
};

/* Shared serialized format; this does not identify either runtime font class. */
struct FontGlyphInfo {
    unsigned char cell_width, cell_height;
    signed char baseline;
    unsigned char max_width;
    unsigned sheet_size;
    unsigned short sheet_count, format;
    unsigned short columns, rows, width, height;
    unsigned char *sheet_image;
};



struct FontTexture {
    unsigned handle;
    struct FontResource *owner;
    unsigned physical_address;
    unsigned short height, width;
    unsigned char format;
    unsigned char reserved11[3];
};

struct FontResource {
    const struct FontVTable *vtable;
    struct FontFile *file;
    struct FontInformation *information;
    struct FontTexture *textures;
    unsigned flags;
    unsigned short cached_character, cached_index;
};

#ifdef __cplusplus
extern "C" {
#endif
extern int func_001149c0(const struct FontFile *file, unsigned signature,
                          unsigned version, unsigned minimum_blocks);
extern struct FontFile *func_0013ea40(struct FontResource *font);
extern struct FontFile *func_0019dbec(struct FontResource *font);
extern int func_0019da14(struct FontResource *font, struct FontFile *file);
extern void *func_0019d9d8(struct FontResource *font, void *textureSheets);
#ifdef __cplusplus
}
#endif


struct GlyphWidths {
    signed char left;
    unsigned char glyph_width;
    signed char advance;
};
struct FontVTable {
    void (*destroy_complete)(struct FontResource *font);
    void (*destroy_delete)(struct FontResource *font);
    int (*width)(const struct FontResource *font);
    int (*height)(const struct FontResource *font);
    int (*ascent)(const struct FontResource *font);
    unsigned char reserved14[0x28];
    struct GlyphWidths (*get_widths)(const struct FontResource *font, unsigned short character);
    unsigned char reserved40[0x28];
    int (*texture_count)(const struct FontResource *font);
};

#ifdef __cplusplus
extern "C" {
int func_001efcec(const FontResource *font, unsigned short character);
int func_001f4994(const FontResource *font, unsigned short character);
}
#endif

#endif
