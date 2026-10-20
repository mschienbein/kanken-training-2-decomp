struct ResourceDataView {
    unsigned reserved00[2];
    const unsigned char *data;
};

/* The adjacent byte accessor dereferences the same data field. */
const unsigned char *func_001efcd8(const struct ResourceDataView *resource)
{
    return resource->data;
}
