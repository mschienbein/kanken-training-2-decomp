#include "game/runtime.h"
struct G103798 { void *p; int n; };
extern struct G103798 data_00242220;
void func_00103798(void) {
    if (data_00242220.p) {
        func_0019dc20(data_00242220.p);
        data_00242220.n = 0;
    }
}

struct E1091d4 { char pad0[0x1c]; int a; char pad1[0x28 - 0x20]; float x; char pad2[4]; float y; int z; char pad3[0x3c - 0x38]; };
extern struct E1091d4 bss_0049c118[];
void func_001091d4(int idx, int a, int x, int y) {
    struct E1091d4 *e = &bss_0049c118[idx];
    e->a = a;
    e->z = 0;
    e->x = (float)x;
    e->y = (float)y;
}

struct S1103d4 { char pad[0x4c]; float d; char pad2[0x5c - 0x50]; float n; };
float func_001103d4(struct S1103d4 *s) { return s->n / s->d; }
