struct WideTextBuffer;

extern unsigned func_0019d5b0(const unsigned short *text);
extern unsigned func_001cf448(struct WideTextBuffer *buffer,
                              const unsigned short *text,
                              unsigned position, unsigned count);

unsigned func_001cf418(struct WideTextBuffer *buffer, const unsigned short *text,
                       unsigned position)
{
    return func_001cf448(buffer, text, position, func_0019d5b0(text));
}
