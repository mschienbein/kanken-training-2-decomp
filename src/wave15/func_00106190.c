#include "game/functions.h"
#include "game/startup.h"





#include "game/screen.h"






extern void func_001073c0(int index, unsigned short priority);
extern void func_00107f0c(void);


extern void func_0010e888(void);

void func_00106190(void)
{
    data_0023fdc8.current = 1;
    data_0023fdc8.saved = 1;
    func_0010aa18(0);
    func_0010a804(0);
    func_0010a828(0);
    func_0010920c(0);
    func_0010a814();
    func_0019eb60(0, 1);
    func_001360dc(0);
    func_0013604c(4);
    func_0013605c(10);
    func_0013606c(1);
    func_0013607c(0);
    func_001073c0(-1, 1);
    func_00107f0c();
    func_001091d4(0, -1, 10, 1);
    func_001091d4(1, -1, 10, 1);
    func_0010e818();
    func_0010e888();
}
