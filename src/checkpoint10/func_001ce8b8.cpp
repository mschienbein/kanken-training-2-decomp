struct BinaryFileHeader;
struct BinaryBlockHeader;
struct LayoutAnimationFile {
    const BinaryFileHeader *file;
    const BinaryBlockHeader *pai1;
    const BinaryBlockHeader *pat1;
    const BinaryBlockHeader *pah1;
};

class LayoutAnimationObject {
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void setResource(const LayoutAnimationFile *reader, void *accessor);
};

/* INTEGRATOR: move to include/game/layout_resources.h */
extern "C" {
void func_001cbf34(LayoutAnimationFile *reader, const void *data);
}

/* [INFERENCE] The pointer-used entry forwards a resource and accessor to slot 0x14;
 * no direct caller establishes a return value. */
extern "C" void func_001ce8b8(LayoutAnimationObject *object,
                               const BinaryFileHeader *file, void *accessor)
{
    LayoutAnimationFile reader;
    LayoutAnimationFile *resource = &reader;
    func_001cbf34(resource, file);
    const LayoutAnimationFile &argument = *resource;
    object->setResource(&argument, accessor);
}
