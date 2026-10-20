#include "game/notification.h"
#include "game/scene_lifecycle.h"



void CalendarScene::depart()
{
    CalendarScene *scene = this;
    if (func_00177124(7) != 0)
        return;
    func_00177084();
    func_001c1abc(scene->small);
    func_001c29ac(scene->display);
    func_001c2d74(scene->wide);
    func_00181b58(1);
    if ((scene->base.index == 7 || scene->base.index == 9) &&
        func_0016f9c0(0x2000) != 0) {
        func_001dc240(0x2000);
        func_00170f20(17);
        func_00174454(17, 0)->count = scene->base.index;
        scene->base.index = 17;
    }
    func_00176e24(&scene->base);
    func_00176df4();
    func_00176dc8(scene->base.index);
}
