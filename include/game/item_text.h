#ifndef GAME_ITEM_TEXT_H
#define GAME_ITEM_TEXT_H

struct ArchiveIndex;
struct ArchiveResourceTable;
struct BankInstrumentInfo;
struct ItemPage;
struct ItemPageSet;
struct MenuItem;
struct SoundArchiveReader;
struct SoundRecord;
struct W12cAlternateTextState;
struct W12cGlyphMetrics;
struct W12cPrimaryTextState;
struct W12cRegionData;
struct W12cRegionReader;
struct W12cSoundDetail;
struct W12cSoundOption;
struct W12cSoundOptionResult;

#include "game/runtime.h"
#include "game/text.h"
#include "game/item_pages.h"
#include "game/archive.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct W12cSoundDetail {
    unsigned signature;
    struct RelativeReference contents;
    unsigned reserved0c[3];
    struct RelativeReference option;
};


struct W12cAlternateTextState {
    unsigned char reserved00[0xcc];
    int item_index;
    unsigned reservedd0;
    int page_index;
    unsigned char reservedd8[0xea];
    unsigned short text[128];
};

struct W12cPrimaryTextState {
    unsigned char reserved00[0xb4];
    int item_index;
    unsigned reservedb8;
    int page_index;
    unsigned char reservedc0[0xc4];
    unsigned short text[128];
};

struct W12cGlyphMetrics {
    const void *texture;
    signed char left;
    unsigned char width;
    signed char advance;
    unsigned char height;
    unsigned short texture_width;
    unsigned short texture_height;
    unsigned short texture_x;
    unsigned short texture_y;
    unsigned short texture_format;
    unsigned char initialized;
    unsigned char reserved13;
    void **texture_cache;
};


struct W12cRegionData {
    unsigned char reserved00;
    signed char value01;
    unsigned char reserved02[5];
    unsigned char value07;
    unsigned char reserved08[0x0c];
    unsigned char value14;
    unsigned char value15;
    unsigned char value16;
};

struct W12cRegionReader {
    unsigned char reserved00[8];
    const struct W12cRegionData *data;
};


struct W12cSoundOption {
    unsigned flags;
    unsigned first;
    unsigned second;
};

struct W12cSoundOptionResult {
    unsigned char enabled;
    unsigned char reserved01[3];
    unsigned first;
    unsigned second;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct MenuItem *func_0017a49c(struct ItemPage *page, int index, int *changed);
extern int func_001eb688(const struct MenuItem *item);
extern const unsigned short *func_001eb694(const struct MenuItem *item, int index);
extern unsigned short *func_0015de60(unsigned short *destination, unsigned capacity_bytes,
                             const unsigned short *source, int count);
extern int func_0016992c(int group, int index, int mask, int value);
extern int func_001dd504(int group, int index, int variant);
extern const unsigned *func_001f22ac(const struct ArchiveResourceTable *info);
extern const struct W12cSoundDetail *func_001f23ec(const struct SoundRecord *record);
extern const struct W12cSoundOption *func_001f22e0(const struct W12cSoundDetail *detail);
extern bool func_001f23b8(const struct BankInstrumentInfo *info);
extern unsigned func_001f2354(const struct BankInstrumentInfo *info);
extern unsigned func_001f2334(const struct BankInstrumentInfo *info);
extern unsigned short *func_00168e80(struct W12cAlternateTextState *state);
extern unsigned short *func_001790a8(struct W12cPrimaryTextState *state);
extern void func_001db138(struct W12cGlyphMetrics *metrics);
extern int func_001dc574(int group, int index, int value);
extern int func_001dc90c(int group, int index, int value);
extern int func_001dd4f8(const struct ItemPageSet *descriptor, int index);
extern int func_001efce0(const struct W12cRegionReader *reader);
extern unsigned func_001efe90(const struct W12cRegionReader *reader);
extern unsigned func_001f0028(const struct W12cRegionReader *reader);
extern unsigned func_001f0034(const struct W12cRegionReader *reader);
extern unsigned func_001f0040(const struct W12cRegionReader *reader);
extern unsigned func_001f2b98(struct ArchiveIndex *reader);
extern int func_001f2e3c(const struct SoundArchiveReader *reader, unsigned sound_id,
                  struct W12cSoundOptionResult *out);
#ifdef __cplusplus
}
#endif

#endif
