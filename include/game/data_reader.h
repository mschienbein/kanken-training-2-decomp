#ifndef GAME_DATA_READER_H
#define GAME_DATA_READER_H



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
class ReaderSource;
struct DataReader;

class ReaderSource {
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual const void *get_data() const;
};
struct DataReader {
    unsigned char reserved00[0x14];
    const ReaderSource *source;
    unsigned first;
    unsigned second;
    unsigned reserved20;
    const void *cached;
};

extern "C" {
const void *func_001f1fb4(const DataReader *reader);
}
#endif

#endif
