struct E1033cc { int a; int b; };
struct G1033cc { struct E1033cc *arr; int n; };
extern struct G1033cc data_0024225c;
extern void *func_0019d798(int kind, int align, int size);
void func_001033cc(int n) {
    int i;
    struct E1033cc *arr = func_0019d798(2, 4, n * 8);
    data_0024225c.arr = arr;
    data_0024225c.n = n;
    for (i = 0; i < data_0024225c.n; i++) {
        arr[i].a = -1;
        arr[i].b = 0;
    }
}
