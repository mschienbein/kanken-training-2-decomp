#include "game/command_state.h"
#include "game/functions.h"
#include "game/scheduler.h"
#include "game/screen.h"



extern void func_00190688(int index, void (*callback)(void));





void func_0011f3a8(void)
{
    int index = func_0019b564();
    if (func_0019b54c(index)) {
        if (!func_0019efd0()) {
            func_00190688(index, func_0012d328);
            return;
        }
    } else {
        func_00186944(index, 1);
        func_001865ec(1, 0x7a);
        func_001865ec(3, 0x34);
        func_001865ec(4, 0x2b);
        func_001865ec(5, 0x5f);
        func_001865ec(6, 0x7d);
        func_001865ec(7, 0x31);
        func_001865ec(8, 0x45);
        func_001865ec(9, 0x21);
        func_001865ec(10, 0x35);
        func_001865ec(11, 0x27);
        func_001865ec(12, 0xb2);
        func_001865ec(13, 13);
        func_001865ec(14, 0xd79);
        func_001865ec(15, 3);
        func_001865ec(16, 0x19);
        func_00186598();
        return;
    }
}
