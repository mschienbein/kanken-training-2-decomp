#ifndef GAME_FONT_RESOURCE_H
#define GAME_FONT_RESOURCE_H

struct BankInstrumentInfo;
struct BinaryBlockHeader;
struct BinaryFileHeader;
struct FontGlyphInfo;
struct FontInfo;
struct FontInformation;
struct FontReader;
struct FontResource;
struct FontResourceHeader;
struct FontResourceReader;
struct FontVTable;
struct P1179b4;
struct S1179b4;
struct SoundFileBuffer;
struct SoundRecord;
struct SoundState001d2c78;
struct SoundUpdateFlag;
struct ValuePresence;
struct VramAllocator;
struct VramNode;
struct WordProperty;
struct FontManager;

#include <stddef.h>
#include "game/pool_list.h"
#include "game/font.h"






















#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct VramNode {
    unsigned char *address;
    unsigned size;
    struct VramNode *next;
};

struct VramAllocator {
    unsigned count[2];
    unsigned char *begin[2];
    unsigned size[2];
    struct VramNode nodes[2][65];
};

struct BinaryFileHeader {
    unsigned signature;
    unsigned short byte_order;
    unsigned short header_size;
    unsigned version;
    unsigned file_size;
    unsigned short block_count;
    unsigned short reserved12;
#ifndef __cplusplus
    unsigned char blocks[];
#endif
};

struct SoundState001d2c78 {
    unsigned char reserved00[0x10];
    unsigned state;
};

struct SoundUpdateFlag {
    unsigned char reserved00[0x0d];
    unsigned char pending;
};

struct FontReader {
    const void *vtable;
    struct BinaryFileHeader *file;
    struct FontInfo *info;
};

struct BinaryBlockHeader {
    unsigned signature;
    unsigned size;
};

struct FontGlyphInfo {
    unsigned char cell_width;
    unsigned char cell_height;
    unsigned char baseline;
    unsigned char max_width;
    unsigned sheet_size;
    unsigned short sheet_count;
    unsigned short format;
    unsigned short columns;
    unsigned short rows;
    unsigned short width;
    unsigned short height;
    unsigned char *sheet_image;
};

struct WordProperty {
    unsigned value;
};

struct P1179b4 { int a, b, c; };

struct S1179b4 { int pad; struct P1179b4 *p; };

struct ValuePresence {
    unsigned reserved00;
    unsigned value;
};

struct FontResourceHeader {
    unsigned char reserved00[0x0c];
    int data_offset;
};

struct FontInformation {
    unsigned char reserved00;
    signed char field01;
    unsigned char reserved02[5];
    unsigned char field07;
    struct FontGlyphInfo *glyphs;
    unsigned char reserved0c[8];
    unsigned char field14;
    unsigned char field15;
    unsigned char field16;
};

struct FontResourceReader {
    const void *vtable;
    const unsigned char *resource;
    const struct FontInformation *information;
};

#ifdef __cplusplus
extern "C" {
#endif
extern int func_0018d280(struct VramAllocator *allocator, unsigned size,
                         unsigned alignment, unsigned bank, int *slot, int commit);
extern int func_001813a0(const struct BinaryFileHeader *file, unsigned signature,
                  unsigned version, unsigned minimum_blocks);
extern const struct FontVTable data_00223d98;
void *func_001c5b84(void *object);
extern void func_001c5b8c(struct FontResource *font);
extern const struct BankInstrumentInfo *func_001f23f8(const struct SoundRecord *record);
extern void *func_001177fc(struct VramAllocator *allocator, unsigned size, unsigned alignment);
extern int func_00149494(struct VramAllocator *allocator, unsigned size, unsigned alignment);
extern int func_001494b8(struct VramAllocator *allocator, unsigned size, unsigned alignment);
extern void func_001d2c78(struct SoundState001d2c78 *object);
extern void func_001d2e04(struct SoundFileBuffer *buffer);
extern void func_001d5c8c(struct SoundUpdateFlag *object);
extern void func_001d89a0(struct SoundUpdateFlag *object);
extern void func_001dae98(struct FontReader *reader, struct BinaryFileHeader *file,
                   struct FontInfo *info);
extern unsigned func_001db354(const struct BinaryFileHeader *file);
extern void func_001107b4(const struct WordProperty *property, unsigned *output);
extern int func_001179c4(struct S1179b4 *object);
extern int func_001179d4(struct S1179b4 *object);
extern void func_0011e97c(const struct WordProperty *property, unsigned *output);
extern int func_001266d4(const struct ValuePresence *object);
extern void func_0012cebc(const struct WordProperty *property, unsigned *output);
extern const unsigned char *func_001f4888(const struct FontResourceHeader *header);
extern int func_001f4988(const struct FontResourceReader *reader);
extern int func_001f4b3c(const struct FontResourceReader *reader);
extern int func_001f4cd4(const struct FontResourceReader *reader);
extern int func_001f4ce0(const struct FontResourceReader *reader);
extern int func_001f4cec(const struct FontResourceReader *reader);
#ifdef __cplusplus
}
#endif


#include "game/render_state.h"
#ifdef __cplusplus
/* [INFERENCE] Native identities and roles; layouts follow matching consumers. */

struct FontManager {
    const void *vtable;
    unsigned char reserved04[0x20];
    PoolList fonts;
    PoolList textures;
};

extern "C" {
extern unsigned char data_00242f8c;
extern void func_0018f0dc(int count, const unsigned *handles);
extern const unsigned data_00224128[];
extern PoolList *func_001cc4cc(PoolList *list);
extern PoolList *func_001cc12c(PoolList *list);

extern void func_001daea4(FontResource *font);
extern void func_001dae08(FontResource *font);
extern FontResource *func_001daf74(FontResource *font);
extern const FontVTable data_00224d30;
}
#endif

#endif
