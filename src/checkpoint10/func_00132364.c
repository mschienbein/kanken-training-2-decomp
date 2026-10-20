extern void func_00137870(const void *input, void *output, unsigned size,
    unsigned input_width, unsigned input_gap, unsigned output_width,
    unsigned output_gap, unsigned flags);

void func_00132364(const void *input, void *output, unsigned size,
    unsigned input_width, unsigned input_gap, unsigned output_width,
    unsigned output_gap, unsigned flags)
{
    func_00137870(input, output, size, input_width, input_gap, output_width,
                  output_gap, flags);
}
