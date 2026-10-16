/* Reading a character precedes advancing its pointer; the pointer itself may
 * alias the input buffer, so this source differs semantically from *(*p)++. */
unsigned int func_001db054(const unsigned short **position)
{
    unsigned int character = **position;
    ++*position;
    return character;
}
unsigned int func_001db068(const unsigned char **position)
{
    unsigned int character = **position;
    ++*position;
    return character;
}
struct Font { const void *vtable; void *resource; };
extern struct Font *func_001daf74(struct Font *font);
extern const void *data_00224d30[];
struct Font *func_001db3ec(struct Font *font)
{
    font = func_001daf74(font);
    font->vtable = data_00224d30;
    return font;
}
