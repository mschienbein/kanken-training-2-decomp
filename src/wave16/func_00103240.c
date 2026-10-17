/* [INFERENCE] The two halfwords are an initial value and a reset counter. */
struct HalfwordState {
    unsigned short initial;
    unsigned short current;
};

extern struct HalfwordState data_00242240;

void func_00103240(unsigned short initial)
{
    data_00242240.initial = initial;
    data_00242240.current = 0;
}
