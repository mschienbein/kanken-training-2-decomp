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
/* FontVTable intentionally remains opaque, following c009-r5. Concrete
 * providers have distinct receiver/result contracts; no callable slots are
 * asserted until the complete font-family recovery. GlyphWidths is only the
 * field record used by the separate c010 width implementation. */

#ifdef __cplusplus
namespace nw { namespace font { struct Glyph; } }
/* [INFERENCE] Target-version glyph protocol, deliberately separate from the
 * native Glyph whose format/update fields occupy different offsets. Uncalled
 * virtual declarations describe ordering only, never recovered signatures. */
class W12cFont {
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void get_glyph(W12cGlyphMetrics *glyph, unsigned short code);
};
struct W12cGlyphWriter {
    unsigned char reserved00[0x24];
    float scale_x, scale_y, cursor_x;
    unsigned char reserved30[8];
    float fixed_advance;
    W12cFont *font;
    unsigned char reserved40[8];
    unsigned char fixed_width;
};
extern "C" {
void func_001d9e0c(W12cGlyphWriter *writer, const W12cGlyphMetrics *metrics, float x);
float func_001da39c(W12cGlyphWriter *writer, unsigned short character);
void func_001efeac(const FontResource *font, nw::font::Glyph *glyph, unsigned short character);
void func_001efee4(const FontResource *font, nw::font::Glyph *glyph, unsigned short index);
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] Constructor-bound native width/map slots retain const/ushort
 * inputs and the signed three-byte width aggregate. The other target font
 * family below deliberately keeps its separately evidenced full-word inputs. */
struct GlyphWidths func_001efd08(const struct FontResource *font, unsigned short character);
unsigned short func_001efd80(const struct FontResource *font, unsigned short character);
unsigned short func_001f4a2c(struct FontResource *font, unsigned character);
void func_001f4b90(struct FontResource *font, struct W12cGlyphMetrics *glyph, unsigned index);
void func_001f4b58(struct FontResource *font, struct W12cGlyphMetrics *glyph, unsigned character);
#ifdef __cplusplus
}
#endif


#endif
