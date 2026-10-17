/* [INFERENCE] The seek/write roles follow the caller's file offset and source
 * buffer. Uncalled virtual slots describe layout only; their signatures are
 * unrecovered and are not evidence for additional callable interfaces. */
class PositionedOutputStream {
public:
    virtual void slot00();
    virtual void slot04();
    virtual void slot08();
    virtual void slot0c();
    virtual void slot10();
    virtual void slot14();
    virtual int seek(long long offset);
    virtual void slot1c();
    virtual void slot20();
    virtual void slot24();
    virtual int write(int *written, const void *buffer, unsigned size, bool flush);
};

/* INTEGRATOR: move to include/game/positioned_output_stream.h */
extern "C" {
    bool func_0018ff34(PositionedOutputStream *stream, const unsigned char *buffer,
                       int position, int buffer_offset, unsigned size);
}

bool func_0018ff34(PositionedOutputStream *stream, const unsigned char *buffer,
                   int position, int buffer_offset, unsigned size)
{
    bool failed = false;
    int written;
    int result = stream->seek(position + buffer_offset);
    if ((unsigned)result & 0x80000000u) {
        failed = true;
    } else {
        result = stream->write(&written, buffer + buffer_offset, size, true);
        if ((unsigned)result & 0x80000000u)
            failed = true;
    }
    return failed;
}
