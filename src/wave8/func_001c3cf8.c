struct TransformTarget;

extern void func_0016bde0(struct TransformTarget *const *objects, int index,
                          int mode, float x, float y);

void func_001c3cf8(struct TransformTarget *const *objects, int index,
                   float x, float y)
{
    func_0016bde0(objects, index, 1, x, y);
}
