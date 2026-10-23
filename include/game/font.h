#ifndef GAME_FONT_H
#define GAME_FONT_H

struct FontInformation;
struct FontResource;
struct FontVTable;
struct W12cGlyphMetrics;

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
    /* Original lookup mutates these two cache halfwords through const font
     * interfaces. They are not an appended decoder/cache subclass. */
#ifdef __cplusplus
    mutable unsigned short cached_code, cached_index;
#else
    unsigned short cached_code, cached_index;
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned func_0019d8c4(const struct FontFile *file);
extern int func_001149c0(const struct FontFile *file, unsigned signature,
                          unsigned version, unsigned minimum_blocks);
extern struct FontFile *func_0013ea40(struct FontResource *font);
extern struct FontFile *func_0019dbec(struct FontResource *font);
extern int func_0019da14(struct FontResource *font, struct FontFile *file);
extern void *func_0019d9d8(struct FontResource *font, void *textureSheets);
#ifdef __cplusplus
}
#endif


/* FontVTable intentionally remains incomplete, as in the accepted interface.
 * Its concrete table providers have distinct receiver/result contracts; no
 * callable members are exposed before the coordinated font-family recovery. */


/* Serialized three-byte width data does not complete the opaque FontVTable. */
struct GlyphWidths {
    signed char left;
    unsigned char glyph_width;
    signed char advance;
};

#endif
