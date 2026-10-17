#ifndef GAME_FORMATTED_TEXT_H
#define GAME_FORMATTED_TEXT_H

struct FormattedText;
struct TextFormatContainer;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
class FormattedText {
public:
    virtual ~FormattedText();
    virtual void reserved02();
    virtual void reserved03();
    virtual void reserved04();
    virtual void reserved05();
    virtual void reserved06();
    virtual void reserved07();
    virtual void reserved08();
    virtual void reserved09();
    virtual void reserved10();
    virtual void reserved11();
    virtual void reserved12();
    virtual void reserved13();
    virtual void reserved14();
    virtual void reserved15();
    virtual void reserved16();
    virtual void reserved17();
    virtual void reserved18();
    virtual void reserved19();
    virtual void reserved20();
    virtual void reserved21();
    virtual void reserved22();
    virtual void reserved23();
    virtual void reserved24();
    virtual void reserved25();
    virtual void reserved26();
    virtual void reserved27();
    virtual void reserved28();
    virtual void reserved29();
    virtual void set_text(const unsigned short *text, unsigned short start);
    unsigned char reserved_bytes04[0xf4];
    unsigned short buffer_bytes;
};
#endif

struct TextFormatContainer {
    unsigned char reserved00[8];
    struct FormattedText *text;
};

#ifdef __cplusplus
extern "C" {
#endif
extern void func_0015dc14(struct TextFormatContainer *container, const unsigned short *format, ...);
#ifdef __cplusplus
}
#endif

#endif
