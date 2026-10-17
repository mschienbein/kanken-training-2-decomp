#ifndef GAME_RESOURCE_FONT_H
#define GAME_RESOURCE_FONT_H

struct AnimationBinding;
struct AnimationMaterial;
struct AnimationPane;
struct FontInformationView;
struct FontSheetView;
struct LayoutColor;
struct LayoutColors140;
struct LayoutMember118;
struct PoolLink;
struct PoolList;
struct ResourceFontView;
struct StreamCapabilityBackend;
struct StreamCapabilityForwarder;
struct StreamSizeBackend;
struct StreamSizeForwarder;
struct U8Archive;
struct U8Directory;
struct U8DirectoryEntry;
struct U8Node;

#include <stdbool.h>
#include "game/pool_list.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct U8Node {
    unsigned type_and_name;
    unsigned parent_or_offset;
    unsigned end_or_size;
};

struct U8Archive {
    const unsigned char *image;
    const struct U8Node *nodes;
    unsigned reserved08;
    int node_count;
    const wchar_t *names;
    unsigned reserved14;
    unsigned current_directory;
};

struct U8Directory {
    struct U8Archive *archive;
    unsigned begin;
    unsigned next;
    unsigned end;
};

struct U8DirectoryEntry {
    struct U8Archive *archive;
    unsigned index;
    const wchar_t *name;
    bool is_directory;
};

#ifdef __cplusplus
class StreamSizeBackend {
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
    virtual unsigned get_size(unsigned long long *size);
};
#endif

struct StreamSizeForwarder {
    const void *vtable;
    unsigned reserved04;
    struct StreamSizeBackend *stream;
};

#ifdef __cplusplus
class StreamCapabilityBackend {
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
    virtual void slot16();
    virtual unsigned query(unsigned *value);
};
#endif

struct StreamCapabilityForwarder {
    const void *vtable;
    struct StreamCapabilityBackend *stream;
};

struct AnimationBinding {
    struct PoolLink link;
    void *transform;
    unsigned short target_index;
    unsigned char enabled;
    unsigned char reserved0f;
};

struct AnimationPane {
    unsigned char reserved00[0x1c];
    struct PoolList animations;
};

struct LayoutColor {
    unsigned rgba;
};

struct LayoutColors140 {
    unsigned char reserved00[0x140];
#ifndef __cplusplus
    struct LayoutColor colors[];
#endif
};

struct AnimationMaterial {
    const void *vtable;
    struct PoolList animations;
};

struct LayoutMember118 {
    unsigned char reserved00[0x118];
    void *member;
};

struct FontSheetView {
    unsigned char width, height;
    signed char baseline;
    unsigned char max_char_width;
    unsigned sheet_size;
    unsigned short sheet_count;
};

struct FontInformationView {
    unsigned char reserved00[8];
    const struct FontSheetView *sheet;
};

struct ResourceFontView {
    const void *vtable;
    void *resource;
    const struct FontInformationView *information;
    void *textures;
    unsigned flags;
};

#ifdef __cplusplus
extern "C" {
#endif
extern const wchar_t data_001cd2ec[];
extern const wchar_t data_001cd2e4[];
extern int func_0016b678(const wchar_t *first, const wchar_t *second);
extern bool func_0016b80c(struct U8Archive *archive, const wchar_t *path,
                    struct U8Directory *directory);
extern bool func_0016b76c(struct U8Directory *directory, struct U8DirectoryEntry *entry);
extern bool func_0016b860(struct U8Archive *archive, const wchar_t *path);
extern bool func_001f18c0(struct U8Directory *directory);
extern int func_0016ba0c(const struct U8Archive *archive, const wchar_t *path);
extern int func_0016b6a4(struct U8Archive *archive, const wchar_t *name);
extern unsigned func_001c456c(struct StreamSizeForwarder *self,
                                  unsigned long long *size);
extern unsigned func_001c4588(struct StreamCapabilityForwarder *self, unsigned *value);
extern struct PoolLink *func_001cd904(struct AnimationPane *pane,
                                struct AnimationBinding *binding);
extern void func_001cf1cc(struct LayoutColors140 *self, unsigned index,
                   const struct LayoutColor *color);
extern struct PoolLink *func_001cfd9c(struct AnimationMaterial *material,
                                struct AnimationBinding *binding);
extern bool func_001d3a10(const struct LayoutMember118 *self);
#ifdef __cplusplus
}
#endif

#endif
