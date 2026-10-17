#ifndef GAME_FONT_H
#define GAME_FONT_H

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

struct FontInformation;
struct FontVTable;
struct FontResource {
    const struct FontVTable *vtable;
    struct FontFile *file;
    struct FontInformation *information;
    void *textureSheets;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct FontFile *func_0013ea40(struct FontResource *font);
extern struct FontFile *func_0019dbec(struct FontResource *font);
extern int func_0019da14(struct FontResource *font, struct FontFile *file);
extern void *func_0019d9d8(struct FontResource *font, void *textureSheets);
#ifdef __cplusplus
}
#endif

#endif
