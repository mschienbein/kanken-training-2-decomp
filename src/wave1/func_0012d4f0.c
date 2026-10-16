struct ArchiveInfo;
typedef unsigned long long EncodedId;

extern unsigned int func_0012d5c4(struct ArchiveInfo *out, unsigned char mode,
                                  const EncodedId *ids, unsigned int count);

unsigned int func_0012d4f0(struct ArchiveInfo *out, unsigned char mode,
                            const EncodedId *ids, unsigned int count)
{
    return func_0012d5c4(out, mode, ids, count);
}
