#include <iostream>
#include <QMessageBox>
#include <QPainter>
#include "cldefs.h"
#include "cltimeline.h"

CLTimeLine::CLTimeLine():
    changed(false),
    _unit(cluDay),
    _minDate(QDateTime::currentDateTime()),
    _maxDate(QDateTime::currentDateTime().addDays(7))
{
}

void CLTimeLine::paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    QRect v = p->viewport();
    calcScale(v);
    if (mainDivCount<2) return;
    int vw = v.width();
    int vh = v.height();
    // OY for debugging
    //p->drawLine(0, -vh/2, 0, vh/2+9);
    // Central horizontal line
    p->drawLine(-vw/2, 0, vw/2+4, 0);
    // Main scale divisions;
    int xPix = -vw/2+LEFT_DIV_MARGIN;
    QDateTime xDate = leftScaleDate; // TODO implement dateForX instead this g-code
    for (int i=0; i<mainDivCount; i++) {
        // Division mark
        p->drawLine(xPix, 0, xPix, -MAIN_DIV_HEIGHT);
        // Division text
        p->drawText(xPix, TEXT_Y, xDate.toString(dateFormat));
        // aux divisions (calc each time because 28, 29,30,31 days in month)
        if (i<mainDivCount-1) {
            int auxDivCount;
            if (_actualUnit==cluHour)
                auxDivCount = 6;
            else if (_actualUnit==cluDay)
                auxDivCount = 24;
            else if (_actualUnit==cluWeek)
                auxDivCount = 7;
            else if (_actualUnit==cluMonth)
                auxDivCount = xDate.date().daysInMonth();
            else if (_actualUnit==cluQuarter)
                auxDivCount = 3;
            else
                auxDivCount = 12; // TODO g-code end
            for (int j=0; j<auxDivCount; j++) {
                int auxDivStep = j*mainDivStep/auxDivCount;
                p->drawLine(xPix+auxDivStep, 0, xPix+auxDivStep, -AUX_DIV_HEIGHT);
            }
        }
        // To next division...
        xPix += mainDivStep;
        if (_actualUnit==cluHour) // TODO g-code begin
            xDate = xDate.addSecs(3600);
        else if (_actualUnit==cluDay)
            xDate = xDate.addDays(1);
        else if (_actualUnit==cluWeek)
            xDate = xDate.addDays(7);
        else if (_actualUnit==cluMonth) // TODO: month and above - variable unit step
            xDate = xDate.addDays(30);
        else if (_actualUnit==cluQuarter)
            xDate = xDate.addDays(90);
        else
            xDate = xDate.addDays(365); // TODO g-code end
    };
}

QRectF CLTimeLine::boundingRect() const
{
    //return QRectF(0, -10, 110, 70);
    return QRectF(0, 0, 10, 20);
}

// Timeline settings
void CLTimeLine::setUnit(const ChronoLineUnit& unit)
{
    changed = true;
    _unit = unit;
}

void CLTimeLine::setMinDate(const QDateTime date)
{
    changed = true;
    _minDate = date;
}

void CLTimeLine::setMaxDate(const QDateTime date)
{
    changed = true;
    _maxDate = date;
}

// Date/time coordinate (viewport x) for date
int  CLTimeLine::xForDate(const QDateTime date, const QRect& r)
{
    if (changed) calcScale(r);
    // TODO
    return 0;
}

ChronoLineUnit CLTimeLine::unit() { return _unit; }
QDateTime CLTimeLine::minDate() { return _minDate; }
QDateTime CLTimeLine::maxDate() { return _maxDate; }

ChronoLineUnit CLTimeLine::actualUnit()
{
    if (_unit==cluAuto) {
        if (_minDate.daysTo(_maxDate)<3)
            _actualUnit = cluHour;
        else
        if (_minDate.daysTo(_maxDate)<28)
            _actualUnit = cluDay;
        else
        if (_minDate.daysTo(_maxDate)<90)
            _actualUnit = cluWeek;
        else
        if (_minDate.daysTo(_maxDate)<365)
            _actualUnit = cluMonth;
        else
        if (_minDate.daysTo(_maxDate)<750)
            _actualUnit = cluQuarter;
        else
            _actualUnit = cluYear;
    }
    else _actualUnit = _unit;
    return _actualUnit;
}


// Calculate range, step, etc.
bool CLTimeLine::calcScale(const QRect& r)
{
    if (_minDate>=_maxDate) return false;
    actualUnit();
    leftScaleDate = _minDate; // TODO adjust to nice scale
    if (_actualUnit==cluHour) {
        mainDivCount = leftScaleDate.secsTo(_maxDate)*3600+1;
        dateFormat = "hh:mm";
    }
    else
    if (_actualUnit==cluDay) {
        mainDivCount = leftScaleDate.daysTo(_maxDate)+1;
        dateFormat = "dd.MM";
    }
    else
    if (_actualUnit==cluWeek) {
        mainDivCount = leftScaleDate.daysTo(_maxDate)/7+1;
        dateFormat = "dd.MM";
    }
    else
        // TODO: month and above - variable unit step
    if (_actualUnit==cluMonth) {
        mainDivCount = leftScaleDate.daysTo(_maxDate)/30+1;
        dateFormat = "MM"; // TODO словами!
    }
    else
    if (_actualUnit==cluQuarter) {
        mainDivCount = leftScaleDate.daysTo(_maxDate)/90+1;
        dateFormat = "MM"; // TODO словами!
    }
    else {
        mainDivCount = leftScaleDate.daysTo(_maxDate)/365+1;
        dateFormat = "yyyy";
    };
    if (mainDivCount<2) return false;
    mainDivStep = (r.width()-LEFT_DIV_MARGIN-RIGHT_DIV_MARGIN) / (mainDivCount-1);
    changed = false;
    return true;
}
