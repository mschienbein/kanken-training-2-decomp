#include "game/functions.h"
struct ServiceClientState {
    unsigned char reserved00[0x10];
    unsigned handle;
};

extern struct ServiceClientState data_00242c18;


int func_0010bc70(unsigned handle)
{
    unsigned saved_handle;
    data_00242c18.handle = handle;
    saved_handle = handle;
    func_00113404(&saved_handle, 0x0b0502c8);
    return 0;
}
