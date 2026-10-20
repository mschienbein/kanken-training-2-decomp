struct ActiveRecord {
    unsigned int reserved00;
    unsigned char active;
};
struct ActiveRecordOwner {
    struct ActiveRecord *record;
};

int func_001f2f0c(const struct ActiveRecordOwner *owner)
{
    if (owner->record)
        return owner->record->active == 0;
    return 0;
}
