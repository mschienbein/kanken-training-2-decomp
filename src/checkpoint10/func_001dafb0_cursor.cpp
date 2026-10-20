/* [INFERENCE] The pointer callback follows the neighboring character-cursor
 * callbacks. The stream object and 16-bit result mirror the native font reader;
 * historical class identity is not established. */
#include "game/audio_voice.h"
unsigned short func_001dafb0(ShiftJisCursorState *cursor)
{
    unsigned short character = cursor->get<unsigned char>();
    if (static_cast<unsigned int>(character - 0x81) < 0x1f || character >= 0xe0) {
        character = (character << 8) | cursor->get<unsigned char>(1);
        cursor->step<unsigned char>(2);
    } else {
        cursor->step<unsigned char>();
    }
    return character;
}
