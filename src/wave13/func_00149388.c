#include "game/audio_voice.h"
/* The dispatcher supplies phase even though this generator does not use it. */
float func_00149388(float phase)
{
    return (float)func_00180418() * (1.0f / 65535.0f);
}
