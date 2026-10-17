struct ShaderGeometryBuffers {
    unsigned char reserved00[0x194];
    unsigned buffers[2];
};

extern const unsigned short data_0021c744[6];
extern const float data_0021c750[4];
extern void func_0018da44(int count, unsigned *handles);
extern void func_00193d64(unsigned target, unsigned handle);
extern void func_0018d8e4(unsigned target, unsigned size, const void *source, unsigned usage);

void func_00116c1c(struct ShaderGeometryBuffers *state)
{
    func_0018da44(2, state->buffers);
    func_00193d64(0x8893, state->buffers[0]);
    func_0018d8e4(0x8893, sizeof(data_0021c744), data_0021c744, 0x88e4);
    func_00193d64(0x8893, 0);
    func_00193d64(0x8892, state->buffers[1]);
    func_0018d8e4(0x8892, sizeof(data_0021c750), data_0021c750, 0x88e4);
    func_00193d64(0x8892, 0);
}
