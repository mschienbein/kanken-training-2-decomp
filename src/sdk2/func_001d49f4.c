struct AdpcmState {
    unsigned short predictor;
    short previous;
    short previous2;
};

struct AdpcmCoefficients {
    short pairs[8][2];
};

extern void func_001c5768(const unsigned char *source, short *destination,
                          const struct AdpcmCoefficients *coefficients,
                          struct AdpcmState *state, int samples);

void func_001d49f4(struct AdpcmState *state,
                   const struct AdpcmCoefficients *coefficients,
                   unsigned samples, const unsigned char *source)
{
    short decoded[14];
    int position;
    for (position = 0; position < samples; position += 14) {
        func_001c5768(source, decoded, coefficients, state, 14);
        source += 8;
    }
}
