#ifndef CLDEFS_H
#define CLDEFS_H

enum ChronoLineUnit {
    cluAuto,
    cluHour,
    cluDay,
    cluWeek,
    cluMonth,
    cluQuarter,
    cluYear
};

// Margin (in pixels) of left scale division
#define LEFT_DIV_MARGIN 10
// Margin (in pixels) of right scale division (minimal)
#define RIGHT_DIV_MARGIN 25
// Height (in pixels) of main scale divisions
#define MAIN_DIV_HEIGHT 15
// Height (in pixels) of auxiliary scale divisions
#define AUX_DIV_HEIGHT 8
// Y coordinate for text output
#define TEXT_Y 15


#endif // CLDEFS_H
