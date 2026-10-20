class W20Font {
public:
    virtual ~W20Font();
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual int GetAscent() const;
    virtual int GetDescent() const;
    virtual int GetBaseline() const;
    virtual int GetCellHeight() const;
    virtual int GetCellWidth() const;
    virtual int GetLineFeed() const;
};

struct W20Point2 { float x, y; };

struct W20TextWriter {
    unsigned reserved00[9];
    float scaleX, scaleY;
    W20Point2 cursor;
    float cursorZ;
    float fixedWidth;
    W20Font *font;
    unsigned reserved40[2];
    bool fixedWidthEnabled;
    unsigned char reserved49[11];
    float lineSpace;
    int tabWidth;
};

extern "C" float func_001f48f4(const W20TextWriter *writer)
{
    return writer->font->GetWidth() * writer->scaleX;
}
