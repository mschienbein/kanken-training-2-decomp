
struct QuadBanks { float positions[128]; float colors[]; };
/* [INFERENCE] One literal root contains two banks separated by 0x200 bytes.
 * The caller's valid quad-slot range remains unrecovered. */
extern struct QuadBanks data_0024226c;

static void set_position(float *destination, float x, float y)
{
    destination[0] = x; destination[1] = y;
}
static void set_row(float *destination, float x, float y, float z, float w)
{
    destination[0] = x; destination[1] = y;
    destination[2] = z; destination[3] = w;
}
struct QuadBankPointers { float *position; float *color; };
static struct QuadBankPointers quad_banks(void)
{
    struct QuadBankPointers result;
    result.position = data_0024226c.positions;
    result.color = data_0024226c.colors;
    return result;
}

void func_001941b0(int quad_slot, float left, float top, float right, float bottom,
                   float red, float green, float blue, float alpha)
{
    struct QuadBankPointers banks = quad_banks();
    int offset = quad_slot * 16;
    float *position = &banks.position[offset];
    float *color = &banks.color[offset];
    set_position(position, left, top);
    set_position(position + 4, left, bottom);
    set_position(position + 8, right, top);
    set_position(position + 12, right, bottom);
    set_row(color, red, green, blue, alpha);
    set_row(color + 4, red, green, blue, alpha);
    set_row(color + 8, red, green, blue, alpha);
    set_row(color + 12, red, green, blue, alpha);
}
