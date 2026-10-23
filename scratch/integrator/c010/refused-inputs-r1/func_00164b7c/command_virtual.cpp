/* [INFERENCE] Class identity and destructor slot; layout follows the initializer
 * and the retained category/command constructor. */
/* INTEGRATOR: differs from scratch/authors/w18b/src/func_00164ba4.cpp:
 * the native virtual layout models the vtable rather than a writable pointer. */
struct CommandAction {
    virtual ~CommandAction();
    unsigned active;
    int category;
    int command;
    const char *text;
    CommandAction();
};

CommandAction::CommandAction()
    : active(0), category(-1), command(-1), text(0)
{
}
