/* [INFERENCE] The caller forwards a member pointer; the helper does not
 * inspect it, so the pointed-to type is not recovered. */
void func_001c1080(void *object)
{
    int i = 0;
    do {
        ++i;
    } while (i < 3);
}
