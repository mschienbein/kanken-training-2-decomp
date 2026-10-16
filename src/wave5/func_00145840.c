#pragma pack(push, 2)
struct SynchronizationState {
    unsigned owner;
    unsigned recursion;
    unsigned short locked;
};
#pragma pack(pop)

struct RequestRecord {
    unsigned kind;
    unsigned flags;
    unsigned short count;
    unsigned short length;
    unsigned short text[0x76c];
    unsigned char flag_ee4;
    unsigned char flag_ee5;
    unsigned char flag_ee6;
    unsigned char auxiliary[0x89];
    int handle;
    unsigned short status;
    struct SynchronizationState synchronization;
};
extern void func_0019051c(struct RequestRecord *record);
extern void func_001904ec(void);
extern void func_001904e0(void);
extern void func_00190450(unsigned char *output, struct RequestRecord *record);

static void copyText(unsigned short *destination, const unsigned short *text,
                     unsigned remaining)
{
    while (*text != 0 && remaining != 0) {
        --remaining;
        *destination++ = *text++;
    }
    *destination = 0;
}

void func_00145840(const unsigned short *text)
{
    unsigned char output;
    struct RequestRecord record;
    func_0019051c(&record);
    record.kind = 1;
    copyText(record.text, text, 0x3b6);
    func_001904ec();
    func_001904e0();
    func_00190450(&output, &record);
}
