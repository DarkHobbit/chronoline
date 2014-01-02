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

enum ChronoLineFlagType {
    clftEvent,
    clftPairBeg,
    clftPairEnd
};


// Margin (in pixels) of left scale division
#define LEFT_DIV_MARGIN      25
// Margin (in pixels) of right scale division (minimal)
#define RIGHT_DIV_MARGIN     10
// Height (in pixels) of main scale divisions
#define MAIN_DIV_HEIGHT      15
// Height (in pixels) of auxiliary scale divisions
#define AUX_DIV_HEIGHT        8
// Y coordinate for text output
#define TEXT_Y               15

// Base height of period view
#define BASE_PERIOD_HEIGHT   25
// Shift of height for each next period
#define PERIOD_HEIGHT_SHIFT   5

// Full height of flag view
#define FLAG_HEIGHT 50
// Width of flag view
#define FLAG_WIDTH 25
// Height of upper flag view part
#define FLAG_SUBHEIGHT 20

#endif // CLDEFS_H
