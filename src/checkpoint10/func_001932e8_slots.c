#include <string.h>

extern const char *func_0012142c(unsigned region);
extern const char *func_00121414(unsigned language);
extern int func_0012251c(char *destination, const char *format, ...);

void func_001932e8(char *destination, unsigned kind, unsigned language, unsigned region)
{
    char name[64];
    const char *format;
    const char *language_name;
    const char *region_name;
    unsigned name_kind;
    unsigned language_index;
    language_index = language;
    region_name = func_0012142c(region);
    name_kind = kind;
    language_name = func_00121414(language_index);
    /* ORIGINAL BUG: an unsupported kind passes a null format to the formatter. */
    switch (name_kind) {
    case 0:
        format = "NA_NAME_%s_%s";
        break;
    case 1:
        format = "NA_SORT_NAME_%s_%s";
        break;
    case 2:
        format = "NA_DESCRIPTION_SHORT_%s_%s";
        break;
    case 3:
        format = "NA_DESCRIPTION_LONG_%s_%s";
        break;
    case 4:
        format = "NA_DISCLAIMER_%s_%s";
        break;
    case 5:
        format = "NA_ICON_INDEX_%s_%s";
        break;
    default:
        format = 0;
        break;
    }
    /* ORIGINAL BUG: the whole local array, including bytes after its terminator, is copied. */
    func_0012251c(name, format, language_name, region_name);
    memcpy(destination, name, sizeof(name));
}
