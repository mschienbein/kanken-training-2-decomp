struct AnimationParameters {
    int first;
    int second;
    int third;
    int fourth;
    int fifth;
    float start;
    float finish;
    float offset;
    float velocity;
    float acceleration;
};

struct AnimationParameters *func_00164bcc(struct AnimationParameters *parameters,
    int first, int second, int third, int fourth, int fifth,
    float start, float finish, float offset, float velocity, float acceleration)
{
    parameters->first = first;
    parameters->second = second;
    parameters->third = third;
    parameters->fourth = fourth;
    parameters->fifth = fifth;
    parameters->start = start;
    parameters->finish = finish;
    parameters->offset = offset;
    parameters->velocity = velocity;
    parameters->acceleration = acceleration;
    return parameters;
}
