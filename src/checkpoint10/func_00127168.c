static unsigned luminance(unsigned red, unsigned green, unsigned blue)
{
    /* ORIGINAL BUG: preserve the forced luminance low bit. */
    return (blue & 1) | ((green + (red >> 1) + (blue >> 1)) >> 1);
}
unsigned func_00127168(unsigned color, unsigned format)
{
    unsigned alpha = color >> 24;
    unsigned blue = (color >> 16) & 255;
    unsigned green = (color >> 8) & 255;
    unsigned red = color & 255;
    switch (format) {
    case 0:
        return alpha | ((red << 24) | (green << 16) | (blue << 8));
    case 1:
        return (red << 16) | (green << 8) | blue;
    case 2:
        return (((red >> 3) << 11) | ((green >> 3) << 6) | ((blue >> 3) << 1)) | (alpha > 127 ? 1u : 0u);
    case 3:
        return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
    case 4:
        {
            unsigned packed = (red >> 4) << 12;
            packed |= (green >> 4) << 8;
            packed |= blue & ~15u;
            return (alpha >> 4) | packed;
        }
    case 5:
        /* ORIGINAL BUG: the blue low bit is ORed into the weighted luminance. */
        return (luminance(red, green, blue) << 8) | alpha;
    case 6:
        return green | (red << 8);
    case 7:
        /* ORIGINAL BUG: preserve the original luminance's forced low bit. */
        return ((green + (red >> 1) + (blue >> 1)) >> 1) | (blue & 1);
    case 8:
        return alpha;
    case 9:
        /* ORIGINAL BUG: use the same original luminance before quantization. */
        return (luminance(red, green, blue) & 0xf0) | (alpha >> 4);
    default:
        return 0;
    }
}
