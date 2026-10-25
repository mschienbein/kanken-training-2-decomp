#include "game/applet.h"
#include <stdbool.h>

/* The sole original caller forwards the complete receive event unchanged. */

void func_00115628(int event)
{
    data_00243034.event = (unsigned char)event;
}
