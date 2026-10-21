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

struct FontGlyphData {
    unsigned char cellWidth, cellHeight, baseline, maximumWidth;
    unsigned sheetSize;
    unsigned short sheetCount, sheetFormat;
    unsigned short sheetRows, sheetColumns, sheetWidth, sheetHeight;
    void *sheetImages;
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
    unsigned short reserved14, reserved16;
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


/* FontVTable intentionally remains incomplete, as in the accepted interface.
 * Its concrete table providers have distinct receiver/result contracts; no
 * callable members are exposed before the coordinated font-family recovery. */

#endif
