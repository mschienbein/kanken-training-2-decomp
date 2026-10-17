struct SoundFileCache {
    unsigned char reserved00[4];
};

class SoundFileProvider {
public:
    virtual void reserved00();
    virtual void reserved01();
    virtual void reserved02();
    virtual void reserved03();
    virtual void reserved04();
    virtual const void *get_file(unsigned file_id);
    unsigned char reserved_fields04[8];
    SoundFileCache cache;
};

/* The returned resource is read as CSEQ or CWAR by the callers. */
extern "C" const void *func_0016a92c(SoundFileProvider *provider, unsigned file_id)
{
    return provider->get_file(file_id);
}
