struct Matrix34 {
    float elements[12];
};
struct MatrixRow {
    float elements[4];
};
struct TransformRecorder {
    unsigned char reserved00[0x25];
    unsigned char next_row;
    unsigned char reserved26[0x632];
    float recorded_row;
    unsigned char reserved65c[0x10];
    struct MatrixRow *rows;
};
extern struct Matrix34 *func_00195334(struct Matrix34 *destination, const struct Matrix34 *source);

void func_001ce634(struct TransformRecorder *recorder, const struct Matrix34 *matrix)
{
    func_00195334((struct Matrix34 *)(recorder->rows + recorder->next_row), matrix);
    recorder->recorded_row = (float)recorder->next_row;
    recorder->next_row += 3;
}
