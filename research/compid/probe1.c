typedef unsigned char u8;

struct S10e664 { char pad[0x1c]; int x1c; };
void func_0010e664(struct S10e664 *s) { s->x1c = 0; }

struct S1149a8 { int a, b, c; u8 d; };
void func_001149a8(struct S1149a8 *s) { s->a = 0; s->b = 0; s->c = 0; s->d = 0; }

int func_00136930(int c, int next) {
    if (c == '\n') return 1;
    if (c != '\r') return 0;
    if (next == '\n') return 2;
    return 1;
}

struct E140dac { int a, b, c; };
struct H140dac { char pad[0xc]; struct E140dac e[1]; };
struct S140dac { int pad0; struct H140dac *p; int pad8; u8 flag; };
int func_00140dac(struct S140dac *s, int i) { if (!s->flag) return 0; return s->p->e[i].a; }

struct P1179b4 { int a, b; };
struct S1179b4 { int pad; struct P1179b4 *p; };
int func_001179b4(struct S1179b4 *s) { return s->p ? s->p->b : 0; }

struct Q1263b8 { char pad[0x14]; int x; };
struct S1263b8 { struct Q1263b8 *p; };
int func_001263b8(struct S1263b8 *s) { return s->p->x != 0; }

int func_00122350(int i) { return i * 0x4c + 0x30; }
