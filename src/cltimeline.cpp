#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsWidget>
#include <QMessageBox>
#include <QPainter>
#include "cldefs.h"
#include "cltimeline.h"

ChronoLineUnit parentUnit[cluNone] = {
    cluNone, // n/a
    cluDay,  // from hours
    cluMonth,// from days
    cluMonth,// from weeks
    cluYear, // from months
    cluYear, // from quarters
    cluNone  // from years
};

QString dateFormatString[cluNone+1] = {
    "", // n/a
    "hh:mm", // hours
    "dd", // days
/*    "dd.MM", // days */
    "dd.MM", // weeks
    "MMMM", // months
    "MMMM", // quarters
    "yyyy", // years
    "" // n/a
};

QString d2c(const QDateTime& d) { return d.toString("dd.MM.yyyy hh:mm:ss"); }

CLTimeLine::CLTimeLine():
    _unit(cluDay),
    _actualUnit(cluDay),
    _minDate(QDateTime::currentDateTime()),
    _maxDate(QDateTime::currentDateTime().addDays(7)),
    changed(false),
    selectedObject(0)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

void CLTimeLine::paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    QRect v = p->viewport();
    calcScale(v);
    if (mainDivCount<2) return;
    // Central horizontal line
    int vw = v.width();
    p->drawLine(-vw/2, 0, vw/2+4, 0);
    // Bounds of paint area
    QPen linePen(Qt::blue);
    linePen.setStyle(Qt::DotLine);
    p->setPen(linePen);
    int vh = v.height();
    p->drawLine(-vw/2+LEFT_DIV_MARGIN, -vh/2, -vw/2+LEFT_DIV_MARGIN, vh/2+9);
    p->drawLine(0, -vh/2, 0, vh/2+9);
    p->drawLine(vw/2-RIGHT_DIV_MARGIN, -vh/2, vw/2-RIGHT_DIV_MARGIN, vh/2+9);
    p->setPen(Qt::black);
    // Main scale divisions;
    int xPix;
    QDateTime xDate;
    for (int i=0; i<(mainDivCount+2); i++) {
        xDate = addUnits(_leftScaleDate, i-1);
        xPix = xForDate(xDate, v);
        // Division mark
        p->drawLine(xPix, 0, xPix, -MAIN_DIV_HEIGHT);
        // aux divisions (calc each time because 28, 29,30,31 days in month)
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
            auxDivCount = 12;
        // Children division marks
        if (mainDivStep/auxDivCount>2) // prevent merging neighbor divisions
        for (int j=0; j<auxDivCount; j++) {
            int auxDivStep = j*mainDivStep/auxDivCount;
            p->drawLine(xPix+auxDivStep, 0, xPix+auxDivStep, -AUX_DIV_HEIGHT);
        }
        // Division text
        drawDate(p, xDate, 1, _actualUnit, false);
        // To next division...
        xPix += mainDivStep;
    };
    // Left (full) division text
    xDate = addUnits(_leftScaleDate, 1);
    drawDate(p, xDate, 1, _actualUnit, true);
    // Right (full) division text
    xDate = addUnits(_leftScaleDate, mainDivCount-2);
    drawDate(p, xDate, 1, _actualUnit, true);
}

QRectF CLTimeLine::boundingRect() const
{
    return QRectF(-rect.width()/2, -rect.height()/2, rect.width(), rect.height());
}

void CLTimeLine::drawDate(QPainter *p, const QDateTime& date, short level, ChronoLineUnit nextUnit, bool forceDrawParent)
{
    QRect v = p->viewport();
    int x = xForDate(date, v);
    p->drawText(x, level*TEXT_Y, roundToUnit(date).toString(dateFormatString[nextUnit]));
    // Parent unit text
    if (parentTextNeeded(date, parentUnit[nextUnit])||(forceDrawParent&&(nextUnit<cluYear)))
        drawDate(p, date, level+1, parentUnit[nextUnit], forceDrawParent);
//    if (nextUnit==cluYear) p->drawText(x, (level+1)*TEXT_Y, date.toString("hh:mm")); // debug of precision output
}

// Timeline settings
bool CLTimeLine::setUnit(const ChronoLineUnit& unit)
{
    changed = true;
    _unit = unit;
    return true;
}

bool CLTimeLine::setMinDate(const QDateTime& date, bool checkRange)
{
    if (checkRange && (unitsTo(date, _maxDate)<MIN_UNITS_NUM)) return false;
    changed = true;
    _minDate = date;
    return true;
}

bool CLTimeLine::setMaxDate(const QDateTime& date, bool checkRange)
{
    if (checkRange && (unitsTo(_minDate, date)<MIN_UNITS_NUM)) return false;
    changed = true;
    _maxDate = date;
    return true;
}

bool CLTimeLine::setRange(const QDateTime& minDate, const QDateTime& maxDate, bool checkRange)
{
    if (checkRange && (unitsTo(minDate, maxDate)<MIN_UNITS_NUM)) return false;
    changed = true;
    _minDate = minDate;
    _maxDate = maxDate;
    return true;
}

// Date/time coordinate (viewport x) for date
int  CLTimeLine::xForDate(const QDateTime date, const QRect& r)
{
    if (changed) calcScale(r);
    return x0+(xN-x0)*unitsTo(_minDate, date)/unitsTo(_minDate, _maxDate);
}

QDateTime CLTimeLine::dateForX(int x)
{
    QDateTime d = addUnits(_minDate, unitsTo(_minDate, _maxDate)*((float)x-x0)/((float)xN-x0));
    return d;
}

ChronoLineUnit CLTimeLine::unit() { return _unit; }
QDateTime CLTimeLine::minDate() { return _minDate; }
QDateTime CLTimeLine::maxDate() { return _maxDate; }
QDateTime CLTimeLine::leftScaleDate() { return _leftScaleDate; }
int CLTimeLine::xMin() { return x0; }
int CLTimeLine::xMax() { return xN; }

ChronoLineUnit CLTimeLine::actualUnit()
{
    ChronoLineUnit oldUnit = _actualUnit;
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
    if (_actualUnit!=oldUnit) emit actualUnitChanged(_actualUnit);
    return _actualUnit;
}

// Calculate range, step, etc.
bool CLTimeLine::calcScale(const QRect& r)
{
    rect = r;
    actualUnit();
    if (_minDate>_maxDate) return false;
    if (unitsTo(_minDate, _maxDate)<MIN_UNITS_NUM) return false;
    _leftScaleDate = truncToUnit(_minDate);
    changed = false;
    mainDivCount = (int)unitsTo(_leftScaleDate, _maxDate)+1;
    if (mainDivCount<2) return false;
    x0 = -r.width()/2+LEFT_DIV_MARGIN;
    xN = r.width()/2-RIGHT_DIV_MARGIN;
    mainDivStep = xForDate(addUnits(_leftScaleDate, 1), r)-xForDate(_leftScaleDate, r);
    return true;
}

// D/t length beetwen two dates in selected unit (daysTo() and secsTo()-like)
float CLTimeLine::unitsTo(const QDateTime& baseDate, const QDateTime& newDate, ChronoLineUnit unit)
{
    if (unit==cluAuto) unit = _actualUnit;
    if (unit==cluHour)
        return (float)baseDate.secsTo(newDate)/3600;
    else
    if (unit==cluDay)
        return (float)baseDate.secsTo(newDate)/3600/24;
    else
    if (unit==cluWeek)
        return (float)baseDate.daysTo(newDate)/7;
    else
        // TODO: month and above - variable unit step
    if (unit==cluMonth)
        return (float)baseDate.daysTo(newDate)/30;
    else
    if (unit==cluQuarter)
        return (float)baseDate.daysTo(newDate)/90;
    else  // cluYear
        return (float)baseDate.daysTo(newDate)/365;
}

// D/t add num units to baseDate
QDateTime CLTimeLine::addUnits(const QDateTime& baseDate, float num, ChronoLineUnit unit)
{
    if (unit==cluAuto) unit = _actualUnit;
    if (unit==cluHour)
        return baseDate.addSecs(num*3600);
    else if (unit==cluDay)
        return baseDate.addSecs(num*3600*24);
    else if (unit==cluWeek)
        return baseDate.addDays(num*7);
    else if (unit==cluMonth) // TODO: month and above - variable unit step
        return baseDate.addDays(num*30);
    else if (unit==cluQuarter)
        return baseDate.addDays(num*90);
    else
        return baseDate.addDays(num*365);
}

// D/t truncate date to actual unit (drop minutes if unit is hour, etc.)
QDateTime CLTimeLine::truncToUnit(const QDateTime& baseDate, ChronoLineUnit unit)
{
    if (unit==cluAuto) unit = _actualUnit;
    if (unit==cluHour)
        return QDateTime(baseDate.date(), QTime(baseDate.time().hour(), 0, 0));
    else if (unit==cluDay)
        return QDateTime(baseDate.date());
    else if (unit==cluWeek)
        return baseDate; // TODO adjust to nice scale
    else if (unit==cluMonth)
        return QDateTime(QDate(baseDate.date().year(), baseDate.date().month(), 1));
    else if (unit==cluQuarter)
        return baseDate; // TODO adjust to nice scale
    else
        return QDateTime(QDate(baseDate.date().year(), 1, 1));
}

// D/t round date to actual unit (to next, if >=0.5)
QDateTime CLTimeLine::roundToUnit(const QDateTime& baseDate, ChronoLineUnit unit)
{
    if (unit==cluAuto) unit = _actualUnit;
    QDateTime dFloor = truncToUnit(baseDate, unit);
    QDateTime dCeil = addUnits(dFloor, 1);
    if (unitsTo(dFloor, baseDate)>=0.5*unitsTo(dFloor, dCeil))
        return dCeil;
    else
        return dFloor;
}

void CLTimeLine::zoomIn(float centerRate)
{
    int range = _minDate.secsTo(_maxDate);
    setRange(_minDate.addSecs(centerRate*ZOOM_STEP*range),
             _maxDate.addSecs(-(1-centerRate)*ZOOM_STEP*range), true);
}

void CLTimeLine::zoomOut(float centerRate)
{
    int range = _minDate.secsTo(_maxDate);
    setRange(_minDate.addSecs(-centerRate*ZOOM_STEP*range),
             _maxDate.addSecs((1-centerRate)*ZOOM_STEP*range), true);
}

// check if parent unit text draw needed
bool CLTimeLine::parentTextNeeded(const QDateTime& d, ChronoLineUnit nextUnit)
{
    if (nextUnit==cluHour)
        return d.time().minute()==0;
    else if (nextUnit==cluDay)
        return d.time().hour()==0;
    else if (nextUnit==cluMonth)
        return d.date().day()==1;
    else if (nextUnit==cluYear)
        return d.date().month()==1;
    else return false;
}

void CLTimeLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    oldDragX = event->scenePos().x();
    scene()->update();
    QPointF scPos = event->scenePos();
    QDateTime scDate = dateForX(scPos.x());
    emit mouseMovedOnScene(scPos, scDate);
}

void CLTimeLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Set new position
    int newDragX = event->scenePos().x();
    // Recalc dates
    setRange(addUnits(minDate(), (float)(oldDragX-newDragX)/mainDivStep),
             addUnits(maxDate(), (float)(oldDragX-newDragX)/mainDivStep), true);
    // preparing for next move
    oldDragX = newDragX;
    emit needUpdateAll();
    QPointF scPos = event->scenePos();
    QDateTime scDate = dateForX(scPos.x());
    emit mouseMovedOnScene(scPos, scDate);
}

void CLTimeLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
