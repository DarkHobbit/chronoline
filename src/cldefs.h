#ifndef CLDEFS_H
#define CLDEFS_H

enum ChronoLineUnit {
    cluAuto,
    cluHour,
    cluDay,
    cluWeek,
    cluMonth,
    cluQuarter,
    cluYear,
    cluNone // not using
};

enum ChronoLineFlagType {
    clftEvent,
    clftPairBeg,
    clftPairEnd
};

// Months in quarter - really constant
#define MONTHS_IN_QUARTER 3

// Margin (in pixels) of left scale division
#define LEFT_DIV_MARGIN      25
// Margin (in pixels) of right scale division (minimal)
#define RIGHT_DIV_MARGIN     25
// Height (in pixels) of main scale divisions
#define MAIN_DIV_HEIGHT      15
// Height (in pixels) of auxiliary scale divisions
#define AUX_DIV_HEIGHT        8
// Y coordinate for text output
#define TEXT_Y               15
// Step for scale zoom in and zoom out (relatively to full scale range)
#define ZOOM_STEP 0.1
// Minimum number of units per scale for valid view
#define MIN_UNITS_NUM 2

// Base height of period view
#define BASE_PERIOD_HEIGHT   25
// Shift of height for each next period
#define PERIOD_HEIGHT_SHIFT   5

// Full height of flag view
#define FLAG_HEIGHT 60
// Width of flag view
#define FLAG_WIDTH 25
// Height of upper flag view part
#define FLAG_SUBHEIGHT 20

// Datetime shift (in main units) while flag dragging outside scale
// Minimal units
#define MIN_FLAGDRAG_DATE_SHIFT 0.02
// Maximal units
#define MAX_FLAGDRAG_DATE_SHIFT 0.1
// Shift period, msec
#define FLAGDRAG_DATE_SHIFT_PERIOD 50

#define ODATE(d) (d).toString().toLocal8Bit().data()

#endif // CLDEFS_H
