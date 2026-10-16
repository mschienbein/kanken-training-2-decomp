struct VT10c498 { void *pad[2]; int (*get8)(void *); int (*getC)(void *); };
struct O10c498 { struct VT10c498 *vt; };
struct S10c498 { char pad[0x24]; float a; float b; char pad2[0x3c - 0x2c]; struct O10c498 *o; };
void func_0010c498(struct S10c498 *s, float x, float y) {
    float fx = x / (float)s->o->vt->get8(s->o);
    float fy = y / (float)s->o->vt->getC(s->o);
    s->a = fx;
    s->b = fy;
}
