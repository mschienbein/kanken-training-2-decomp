#include "game/selection.h"
#include "game/item_pages.h"
#include "game/item_text.h"
#include "game/text.h"
#include "game/notification.h"
#include "game/character_panel.h"
#include "game/screen_operations.h"
/* [INFERENCE] The item identifier/category accessors may have been armlink-inlined
 * and removed; these supplied operations do not establish historical identities. */

/* [INFERENCE] Field roles below extend the established menu-scene prefix. */
struct W10ExerciseState {
    unsigned char reserved00[0x14];
    struct W10DrawingInput *input;
    unsigned char reserved18[0x34];
    struct MenuControlSet *list;
    unsigned char reserved50[0x2c];
    struct CharacterPanel *widgets;
    unsigned char reserved80[0x10];
    int action_index;
    unsigned int reserved94;
    int actions[8];
    int busy;
    unsigned char reservedbc[8];
    int mode;
    unsigned int reservedc8;
    int item;
    unsigned int reservedd0;
    int page;
    int selected;
    unsigned int reserveddc;
    int bound;
    unsigned char reservede4[0xd8];
    unsigned char suppress_results;
    unsigned char reserved1bd[5];
    unsigned short answer;
    unsigned char reserved1c4[0x20c];
    int phase;
};



static struct ItemPage *current_page(const struct W10ExerciseState *state)
{
    struct ItemPageSet *set = data_0023ecf4;
    int page = state->page;
    return func_0017a4e8(set, page);
}

static struct MenuItem *current_item(const struct W10ExerciseState *state)
{
    return func_0017a49c(current_page(state), state->item, 0);
}

void func_001ddde4(struct W10ExerciseState *state, int result)
{
    struct ItemPage *page;
    struct MenuItem *item;
    int grade;
    int identifier;
    int count;
    int index;
    int value;
    if (state->mode == 3) {
        struct TransformEntry *record = func_00174454(10, 0);
        if (result == 1)
            record->count++;
        return;
    }
    /* ORIGINAL BUG: the original dereferences page/item lookup results unchecked. */
    page = current_page(state);
    item = current_item(state);
    grade = func_001795bc();
    identifier = r41279_item_identifier(current_item(state));
    func_001dc918(grade, identifier, -1);
    if (result == 1)
        func_001dc744(grade, identifier, -1);
    func_0015f840(page, state->item, result != 0);
    if (state->mode == 1) {
        count = func_001dc150();
        if (count < 999)
            func_001dc170(count + 1);
        if (result == 1) {
            count = func_001dc160();
            if (count < 999)
                func_001dc230(count + 1);
            func_001dbfdc(grade, r41279_item_category(item), identifier);
        } else {
            func_001dbcec(grade, r41279_item_category(item), identifier);
            goto copy_values;
        }
    }
    if (result == 1)
        func_001dbbd4(0);
copy_values:
    if (!state->suppress_results) {
        for (index = 0; index < 7; ++index) {
            value = func_0016e03c(state->widgets, index);
            if (value == 0)
                break;
            func_0015f724(page, state->item, index, value);
        }
    }
}
