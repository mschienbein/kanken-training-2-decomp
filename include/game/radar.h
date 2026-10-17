#ifndef GAME_RADAR_H
#define GAME_RADAR_H

union RadarFloatWord {
    float value;
    int word;
};

struct RadarColor {
    float red;
    float green;
    float blue;
    union RadarFloatWord alpha;
};

struct RadarChart {
    unsigned buffers[2];
    unsigned flags;
    float center_x;
    float center_y;
    float radius;
    int count;
    union RadarFloatWord alpha;
    union RadarFloatWord scale;
    float values[16];
    unsigned char reserved64[8];
    float coordinates[48];
    struct RadarColor colors[17];
#ifndef __cplusplus
    unsigned short indices[];
#endif
};

struct RadarChartSlot {
    unsigned char reserved00[0x28];
    struct RadarChart *chart;
};

struct RadarWidget {
    unsigned reserved00[0x25]; /* retain four-byte prefix alignment in C and C++ */
#ifndef __cplusplus
    struct RadarChartSlot *slots[];
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct RadarChart *func_00167cec(const struct RadarWidget *widget, int index);
extern void func_00167918(const struct RadarWidget *widget, int index, int draw);
#ifdef __cplusplus
}
#endif

#endif
