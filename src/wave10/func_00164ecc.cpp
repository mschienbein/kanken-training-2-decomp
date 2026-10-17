#include "game/render_parameter.h"

RenderParameter::RenderParameter(float x, float y, unsigned int red,
                                 unsigned int green, unsigned int blue,
                                 unsigned int alpha)
{
    this->x = x;
    this->y = y;
    this->red = red;
    kind = 1;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
}
