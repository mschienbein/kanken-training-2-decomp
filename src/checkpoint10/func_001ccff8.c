/* [INFERENCE] NW4C lyt ARCGetStartAddrInMem (ARCHandle/ARCFileInfo layouts from
 * include/nw/lyt/lyt_Arc.h, copied for this C unit). */
struct ARCHandle {
    const unsigned char *archiveStartAddr;
    void *FSTStart;
    void *fileStart;
    unsigned entryNum;
    void *FSTStringStart;
    unsigned FSTLength;
    unsigned currDir;
};

struct ARCFileInfo {
    const struct ARCHandle *handle;
    unsigned startOffset;
    unsigned length;
};

const void *func_001ccff8(const struct ARCFileInfo *af)
{
    unsigned offset = af->startOffset;
    const struct ARCHandle *handle = af->handle;
    return handle->archiveStartAddr + offset;
}
