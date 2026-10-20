struct DisplayFramebuffer {
    void *left;
    void *right;
    unsigned format;
    unsigned stride;
};
struct DisplayFramebufferPair {
    struct DisplayFramebuffer display[2];
};
extern void func_00137b84(struct DisplayFramebufferPair *output);

void func_00142b8c(struct DisplayFramebufferPair *output)
{
    struct DisplayFramebufferPair current;
    if (output == 0)
        return;
    func_00137b84(&current);
    output->display[0].left = current.display[0].left;
    output->display[0].right = current.display[0].right;
    output->display[1].left = current.display[1].left;
    output->display[1].right = current.display[1].right;
    output->display[0].format = current.display[0].format;
    output->display[1].format = current.display[1].format;
    output->display[0].stride = current.display[0].stride;
    output->display[1].stride = current.display[1].stride;
}
