struct G103798 { void *p; int n; };
extern struct G103798 data_00242220;
extern void func_0019dc20(void *p);
void func_00103798(void) {
    if (data_00242220.p) {
        func_0019dc20(data_00242220.p);
        data_00242220.n = 0;
    }
}

struct E1033cc { int a; int b; };
struct G1033cc { struct E1033cc *arr; int n; };
extern struct G1033cc data_0024225c;
extern void *func_0019d798(int kind, int align, int size);
void func_001033cc(int n) {
    int i;
    data_0024225c.arr = func_0019d798(2, 4, n * 8);
    data_0024225c.n = n;
    for (i = 0; i < n; i++) {
        data_0024225c.arr[i].a = -1;
        data_0024225c.arr[i].b = 0;
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

struct VT10c498 { void *pad[2]; int (*get8)(void *); int (*getC)(void *); };
struct O10c498 { struct VT10c498 *vt; };
struct S10c498 { char pad[0x24]; float a; float b; char pad2[0x3c - 0x2c]; struct O10c498 *o; };
void func_0010c498(struct S10c498 *s, float x, float y) {
    s->a = x / (float)s->o->vt->get8(s->o);
    s->b = y / (float)s->o->vt->getC(s->o);
}
