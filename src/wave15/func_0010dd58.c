struct RuntimeInterface;
extern void func_00116ca4(struct RuntimeInterface *interface);
extern void func_00116cd8(struct RuntimeInterface *interface);
extern void func_0014f7e8(unsigned unused0, unsigned unused1);

void func_0010dd58(struct RuntimeInterface *primary, struct RuntimeInterface *secondary)
{
    /* [INFERENCE] callee identity: inlined by armlink. */
    func_0014f7e8(0, 0x04000048);
    func_00116ca4(primary);
    func_00116cd8(secondary);
}
