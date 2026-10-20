#include "game/functions.h"
#include "game/filesystem.h"


int func_0010bc70(unsigned handle)
{
    unsigned saved_handle;
    data_00242c18.session = handle;
    saved_handle = handle;
    func_00113404(&saved_handle, 0x0b0502c8);
    return 0;
}
