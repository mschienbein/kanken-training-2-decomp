#include "game/screen.h"
#include "game/material.h"


static void set_binding_color(ColoredDisplayBinding *binding, int red, int green, int blue)
{
    if (red == -1) {
        blue = 255;
        green = 255;
        red = 255;
    }
    binding->red = static_cast<unsigned char>(red);
    binding->green = static_cast<unsigned char>(green);
    binding->blue = static_cast<unsigned char>(blue);
    DisplayPane *pane = binding->holder->pane;
    func_0017aa90(pane, static_cast<unsigned char>(red), static_cast<unsigned char>(green),
                  static_cast<unsigned char>(blue), binding->alpha);
}

void func_0015ea0c(BoundDisplayObject *object, int index, int red, int green, int blue)
{
    set_binding_color(reinterpret_cast<ColoredDisplayBinding *>(object->bindings[index]),
                      red, green, blue);
}

