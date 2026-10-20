extern void func_00137adc(const void *input, unsigned input_width,
    unsigned input_height, void *output, unsigned output_width,
    unsigned output_height, unsigned flags);

void func_00132324(const void *input, unsigned input_width,
    unsigned input_height, void *output, unsigned output_width,
    unsigned output_height, unsigned flags)
{
    func_00137adc(input, input_width, input_height, output, output_width,
                  output_height, flags);
}
