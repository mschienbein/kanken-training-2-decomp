struct ScrollPosition {
    unsigned char reserved00[0x50];
    float current;
    unsigned char reserved54[8];
    float target;
    unsigned char reserved60[4];
    ScrollPosition();
};

ScrollPosition::ScrollPosition() : current(0.0f), target(0.0f)
{
}
