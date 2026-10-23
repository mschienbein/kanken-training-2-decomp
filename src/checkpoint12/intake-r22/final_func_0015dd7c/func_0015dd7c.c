/* [INFERENCE] This comparison orders non-ASCII characters as signed bytes. */
int func_0015dd7c(const char *left, const char *right)
{
    while (*left == *right) {
        if (*left == '\0' && *right == '\0')
            return 0;
        ++left;
        ++right;
    }
    return *left - *right;
}
