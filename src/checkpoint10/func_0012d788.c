struct DebugRecordCounts {
    unsigned int messages;
    unsigned int rows;
};
extern struct DebugRecordCounts data_002430e4;

/* Callers ignore the result register; no return-value contract is established. */
void func_0012d788(void)
{
    struct DebugRecordCounts cleared = { 0, 0 };
    data_002430e4 = cleared;
}
