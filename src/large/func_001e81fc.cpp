#include "game/command_state.h"
#include "game/deferred_callback.h"
#include "game/screen.h"
#include "game/selection.h"
#include "game/runtime.h"
#include "game/render_parameter.h"
#include "game/item_entries.h"
#include "game/playback_screen.h"



void ScreenInitializationState::build()
{
    ScreenInitializationState *state = this;
    int i;
    func_0019eb48();
    state->first_options = func_001b6bf4();
    state->second_options = func_001b7e00();
    func_001b6d2c(state->first_options);
    func_001b7f38(state->second_options);
    if (state->primary_list) {
        ParameterValue *values[2];
        ParameterValue *value = static_cast<ParameterValue *>(::operator new(sizeof(ParameterValue), data_002232d5));
        if (value) value->value = 20;
        values[0] = value;
        value = static_cast<ParameterValue *>(::operator new(sizeof(ParameterValue), data_002232d5));
        values[1] = value ? func_001757d0(value) : 0;
        func_00175654(state->primary_list, values);
        ParameterValue **array = values;
        for (i = 0; i < 2; ++i) {
            if (array[i]) {
                /* [INFERENCE] callee identity: inlined by armlink; trivial parameter destructor. */
                func_001852f4(func_001ef884(array[i]));
                array[i] = 0;
            }
        }
    }
    if (state->navigation) {
        ParameterValue *values[2];
        ParameterValue *value = static_cast<ParameterValue *>(::operator new(sizeof(ParameterValue), data_002232d5));
        if (value) value->value = 25;
        values[0] = value;
        value = static_cast<ParameterValue *>(::operator new(sizeof(ParameterValue), data_002232d5));
        values[1] = value ? func_00175648(value) : 0;
        func_001754d0(state->navigation, values);
        ParameterValue **array = values;
        for (i = 0; i < 2; ++i) {
            if (array[i]) {
                /* [INFERENCE] callee identity: inlined by armlink; trivial parameter destructor. */
                func_001852f4(func_001ef884(array[i]));
                array[i] = 0;
            }
        }
    }
    if (state->controls) {
        ArrayOwner *values[2];
        ArrayOwner *owner = static_cast<ArrayOwner *>(::operator new(sizeof(ArrayOwner), data_002232d5));
        ArrayOwner *first = owner;
        if (owner) {
            W11PositionCommand *command = static_cast<W11PositionCommand *>(func_0019d788(sizeof(W11PositionCommand)));
            if (command) command = func_00174d9c(command, 43, 0, 0, 0.0f, 0.0f, 2.0f);
            first = func_00174cc8(owner, 0, 1, command);
        }
        values[0] = first;
        owner = static_cast<ArrayOwner *>(::operator new(sizeof(ArrayOwner), data_002232d5));
        values[1] = owner ? func_00174cb0(owner) : 0;
        func_001748ec(state->controls, values);
        ArrayOwner **array = values;
        for (i = 0; i < 2; ++i) {
            if (array[i]) {
                func_001852f4(func_001748b8(array[i]));
                array[i] = 0;
            }
        }
    }
    if (state->style) {
        VectorParameter *values[2];
        VectorParameter *value = static_cast<VectorParameter *>(::operator new(sizeof(VectorParameter), data_002232d5));
        values[0] = value ? func_00174f04(value, L"Ver 1.1", 16, 3, 0.0f, 0.0f, 1.0f) : 0;
        value = static_cast<VectorParameter *>(::operator new(sizeof(VectorParameter), data_002232d5));
        values[1] = value ? func_00174ef8(value) : 0;
        func_00174df0(&state->style->base, values);
        VectorParameter **array = values;
        for (i = 0; i < 2; ++i) {
            if (array[i]) {
                /* [INFERENCE] callee identity: inlined by armlink; trivial parameter destructor. */
                func_001852f4(func_001ef884(array[i]));
                array[i] = 0;
            }
        }
    }
    func_00186598();
    func_00177b10(state, data_002188d0);
}
