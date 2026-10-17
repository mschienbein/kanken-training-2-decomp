class W20SeekStream {
public:
    virtual ~W20SeekStream();
    virtual void reserved02();
    virtual void reserved03();
    virtual void reserved04();
    virtual void reserved05();
    virtual void reserved06();
    virtual void reserved07();
    virtual void reserved08();
    virtual void reserved09();
    virtual void reserved10();
    virtual void reserved11();
    virtual void reserved12();
    virtual void reserved13();
    virtual void reserved14();
    virtual int size();
    virtual void seek(int offset, int origin);
    unsigned reserved_fields04[4];
    W20SeekStream *source;
    void *buffer;
    unsigned buffer_size;
    unsigned reserved20;
    int position;
};

extern "C" void func_001d3e90(W20SeekStream *stream, int offset, int origin)
{
    unsigned size = stream->buffer ? stream->buffer_size : 0;
    if (stream->buffer && size) {
        switch (origin) {
        case 0: stream->position = offset; break;
        case 1: stream->position += offset; break;
        case 2: stream->position = stream->size() - offset; break;
        }
    } else {
        stream->source->seek(offset, origin);
    }
}
