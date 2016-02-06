#include <math.h>
#include <iostream>
#include <QFontMetrics>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsWidget>
#include <QMessageBox>
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
    "hh", // hours
/*    "hh:mm", // hours */
    "dd", // days
/*    "dd.MM", // days */
    "dd.MM", // weeks
    "MMMM", // months
    "MMMM", // quarters
    "yyyy", // years
    "" // n/a
};

QString d2s(const QDateTime& d)
    { return d.toString("dd.MM.yyyy hh:mm:ss"); }
QDateTime s2d(const QString &s)
    { return QDateTime::fromString(s, "dd.MM.yyyy hh:mm:ss"); }

CLTimeLine::CLTimeLine():
    selectedObject(0),
    _unit(cluAuto),
    _actualUnit(cluDay),
    _minUnit(cluHour),
    _minDate(QDateTime::currentDateTime()),
    _maxDate(QDateTime::currentDateTime().addDays(7)),
    changed(false)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

void CLTimeLine::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
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
    // Min, max, left date (debug only)
    /*p->setPen(Qt::red);
    p->drawLine(xForDate(_minDate, v), -vh/2, xForDate(_minDate, v), vh/2+9);
    p->drawLine(xForDate(_maxDate, v), -vh/2, xForDate(_maxDate, v), vh/2+9);
    p->setPen(Qt::yellow);
    p->drawLine(xForDate(_leftScaleDate, v), -vh/2, xForDate(_leftScaleDate, v), vh/2+9);*/
    // Division's marks and text
    p->setPen(Qt::black);
    int divPerText = calcDivPerText(p, _actualUnit);
    bool lockAuxDiv = false;
    // At first pass, program detect if all auxiliary marks can be properly painted
    // At second pass auxiliary marks are painting, if yes
    for (int pass=1; pass<=2; pass++)
    for (int i=0; i<(mainDivCount+2); i++) {
        QDateTime xDate = addUnits(_leftScaleDate, i-1);
        int xPix = xForDate(xDate, v);
        QDateTime xDateNext = addUnits(_leftScaleDate, i);
        int xPixNext = xForDate(xDateNext, v);
        mainDivStep = xPixNext-xPix;
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
        if (pass==1) {
            if (mainDivStep/auxDivCount<4) {
                lockAuxDiv = true;
                break;
            }
        }
        else
        if (!lockAuxDiv) // prevent merging neighbor divisions
        for (int j=0; j<auxDivCount; j++) {
            int auxDivStep = j*mainDivStep/auxDivCount;
            p->drawLine(xPix+auxDivStep, 0, xPix+auxDivStep, -AUX_DIV_HEIGHT);
        }
        // Division text
        if (i%divPerText==0)
            drawDate(p, xDate, 1, _actualUnit, false);
    };
    // Left (full) division text
    QDateTime xDate = _leftScaleDate;
    if (xForDate(xDate, v)<-vw/2)
        xDate = addUnits(_leftScaleDate, 1);
    if ((xDate.date().day()==xDate.date().daysInMonth())&&calcDivPerText(p, cluMonth)>=2)
        xDate = addUnits(xDate, 1);
    drawDate(p, xDate, 1, _actualUnit, true);
    // Right (full) division text
    xDate = addUnits(_leftScaleDate, mainDivCount-1);
    if (xForDate(xDate, v)+calcDateWidth(p, cluMonth)>vw/2)
        xDate = addUnits(_leftScaleDate, mainDivCount-2);
    drawDate(p, xDate, 1, _actualUnit, true);
}

QRectF CLTimeLine::boundingRect() const
{
    return QRectF(-rect.width()/2, -rect.height()/2, rect.width(), rect.height());
}

void CLTimeLine::setMinUnit(const ChronoLineUnit &unit)
{
    _minUnit = unit;
}

void CLTimeLine::drawDate(QPainter *p, const QDateTime& date, short level, ChronoLineUnit nextUnit, bool forceDrawParent)
{
    QRect v = p->viewport();
    int x = xForDate(date, v);
    p->drawText(x, level*TEXT_Y, truncToUnit(date, nextUnit).toString(dateFormatString[nextUnit]));
    // Parent unit text
    if ((parentTextNeeded(date, parentUnit[nextUnit])&&(x>=-v.width()/2))
            ||(forceDrawParent&&(nextUnit<cluYear)))
        drawDate(p, date, level+1, parentUnit[nextUnit], forceDrawParent);

    /*if (nextUnit==cluYear) { //==> debug of precision output
        p->drawText(x, (level+1)*TEXT_Y, date.toString("dd.MM.yyyy"));
        p->drawText(x, (level+2)*TEXT_Y, date.toString("hh:mm::ss"));
        p->drawText(x, (level+3)*TEXT_Y, date.toString("zzz"));
    }*/
}

int CLTimeLine::calcDivPerText(QPainter *p, ChronoLineUnit unit)
{
    return calcDateWidth(p, unit)/mainDivStep+1;
}

int CLTimeLine::calcDateWidth(QPainter *p, ChronoLineUnit unit)
{
    QFontMetrics* fm = new QFontMetrics(p->font());
    return fm->width(_minDate.toString(dateFormatString[unit]));
}

// Timeline settings
bool CLTimeLine::setUnit(const ChronoLineUnit& unit)
{
    if (unit<_minUnit)
        return false;
    changed = true;
    _unit = unit;
    return true;
}

bool CLTimeLine::setMinDate(const QDateTime& date, bool checkRange)
{
    if (checkRange && !checkDateRange(date, _maxDate)) return false;
    changed = true;
    _minDate = date;
    emit rangeChanged(_minDate, _maxDate);
    return true;
}

bool CLTimeLine::setMaxDate(const QDateTime& date, bool checkRange)
{
    if (checkRange && !checkDateRange(_minDate, date)) return false;
    changed = true;
    _maxDate = date;
    emit rangeChanged(_minDate, _maxDate);
    return true;
}

bool CLTimeLine::setRange(const QDateTime& minDate, const QDateTime& maxDate, bool checkRange)
{
    if (checkRange && !checkDateRange(minDate, maxDate)) return false;
    changed = true;
    _minDate = minDate;
    _maxDate = maxDate;
    emit rangeChanged(_minDate, _maxDate);
    return true;
}

// Date/time coordinate (viewport x) for date
int  CLTimeLine::xForDate(const QDateTime date, const QRect& r)
{
    if (changed) calcScale(r);
    return x0+(double)(xN-x0)*_minDate.secsTo(date)/_minDate.secsTo(_maxDate);
    /*if (actualUnit()<cluMonth)
        return x0+(xN-x0)*unitsTo(_minDate, date)/unitsTo(_minDate, _maxDate);
    else
        return x0+(xN-x0)*unitsTo(_minDate, date, cluHour)/unitsTo(_minDate, _maxDate, cluHour);
        //return x0+(xN-x0)*_minDate.daysTo(date)/_minDate.daysTo(_maxDate);*/
}

QDateTime CLTimeLine::dateForX(int x)
{
    QDateTime d;
    d = _minDate.addSecs((double)_minDate.secsTo(_maxDate)*((float)x-x0)/((float)xN-x0));
    /*if (actualUnit()<cluMonth)
        d = addUnits(_minDate, unitsTo(_minDate, _maxDate)*((float)x-x0)/((float)xN-x0));
    else // Constant DAY length for all big units
        d = addUnits(_minDate, unitsTo(_minDate, _maxDate, cluHour)*((float)x-x0)/((float)xN-x0), cluHour);
        //d = _minDate.addDays(_minDate.daysTo(_maxDate)*((float)x-x0)/((float)xN-x0));*/
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
        if (_minDate.secsTo(_maxDate)<60*60*60)
            _actualUnit = cluHour;
        else
        if (_minDate.daysTo(_maxDate)<28)
            _actualUnit = cluDay;
        else
        if (_minDate.daysTo(_maxDate)<89)
            _actualUnit = cluWeek;
        else
        if (_minDate.daysTo(_maxDate)<365)
            _actualUnit = cluMonth;
        else
        if (_minDate.daysTo(_maxDate)<1050) // was 750, and it locked return from years to quarters
            _actualUnit = cluQuarter;
        else
            _actualUnit = cluYear;
    }
    else _actualUnit = _unit;
    if (_actualUnit<_minUnit)
        _actualUnit = _minUnit;
    if (_actualUnit!=oldUnit)
        emit actualUnitChanged(_actualUnit);
    return _actualUnit;
}

// Calculate range, step, etc.
bool CLTimeLine::calcScale(const QRect& r)
{
    rect = r;
    actualUnit();
    if (_minDate>_maxDate) return false;
    if (!checkDateRange(_minDate, _maxDate)) return false;
    _leftScaleDate = truncToUnit(_minDate);
    changed = false;
    mainDivCount = (int)unitsTo(_leftScaleDate, _maxDate)+1;
    if (mainDivCount<2) return false;
    x0 = -r.width()/2+LEFT_DIV_MARGIN;
    xN = r.width()/2-RIGHT_DIV_MARGIN;
    mainDivStep = xForDate(addUnits(_leftScaleDate, 1), r)-xForDate(_leftScaleDate, r);
    return true;
}

// D/t same as QDate::addMonths, but with float argument
QDateTime CLTimeLine::addMonths(const QDateTime& baseDate, float num)
{
    double numBase;
    double numFrac = modf(num, &numBase);
    QDateTime d = baseDate.addMonths(numBase);
    d = d.addSecs(numFrac*d.date().daysInMonth()*24*3600);
    return d;
}

// D/t length beetwen two dates in months
float CLTimeLine::monthsTo(const QDateTime &baseDate, const QDateTime &newDate)
{
    QDate baseD = baseDate.date();
    if (baseD.month()==newDate.date().month())
        return baseDate.daysTo(newDate)/baseD.daysInMonth();
    else if (baseDate>newDate)
        return -(this->monthsTo(newDate, baseDate));
    else {
        QDateTime d = baseDate;
        float res = 0;
        while (d.addDays(d.date().daysInMonth())<=newDate) {
            res++;
            d = d.addDays(d.date().daysInMonth());
        }
        res += (float)d.daysTo(newDate)/d.date().daysInMonth();
        return res;
    }
}

// D/t length beetwen two dates in selected unit (daysTo() and secsTo()-like)
float CLTimeLine::unitsTo(const QDateTime& baseDate, const QDateTime& newDate, ChronoLineUnit unit)
{
    if (unit==cluAuto) unit = _actualUnit;
    if (unit==cluHour)
        return (float)baseDate.secsTo(newDate)/3600;
    else if (unit==cluDay)
        return (float)baseDate.secsTo(newDate)/3600/24;
    else if (unit==cluWeek)
        return (float)baseDate.secsTo(newDate)/3600/24/7;
    // Month and above - variable unit step
    else if (unit==cluMonth)
        return monthsTo(baseDate, newDate);
    else if (unit==cluQuarter)
        return monthsTo(baseDate, newDate)/MONTHS_IN_QUARTER;
    else  // cluYear
        return monthsTo(baseDate, newDate)/12;
}

// D/t add num units to baseDate
QDateTime CLTimeLine::addUnits(const QDateTime& baseDate, float num, ChronoLineUnit unit)
{
    if (unit==cluAuto) unit = _actualUnit;
    if (unit==cluHour)
        return baseDate.addSecs(num*3600);
    else if (unit==cluDay)
        return baseDate.addSecs(num*3600*24); // if use addDays here, mouse move is very discrete
    else if (unit==cluWeek)
        return baseDate.addSecs(num*3600*24*7); // see above
    else if (unit==cluMonth)
        return this->addMonths(baseDate, num);
    else if (unit==cluQuarter)
        return this->addMonths(baseDate, num*MONTHS_IN_QUARTER);
    else
        return this->addMonths(baseDate, num*12);
}

// D/t truncate date to actual unit (drop minutes if unit is hour, day if unit is month, etc.)
QDateTime CLTimeLine::truncToUnit(const QDateTime& baseDate, ChronoLineUnit unit)
{
    if (unit==cluAuto) unit = _actualUnit;
    if (unit==cluHour)
        return QDateTime(baseDate.date(), QTime(baseDate.time().hour(), 0, 0));
    else if (unit==cluDay)
        return QDateTime(baseDate.date());
    else if (unit==cluWeek) {
        QDateTime d(baseDate.date());
        while (d.date().dayOfWeek()!=1)
            d = d.addDays(-1);
        return d;
    }
    else if (unit==cluMonth)
        return QDateTime(QDate(baseDate.date().year(), baseDate.date().month(), 1));
    else if (unit==cluQuarter)
        return QDateTime(QDate(baseDate.date().year(),
            ((baseDate.date().month()-1)/MONTHS_IN_QUARTER)*MONTHS_IN_QUARTER+1, 1));
    else
        return QDateTime(QDate(baseDate.date().year(), 1, 1));
}

// D/t ceil date to actual unit
QDateTime CLTimeLine::ceilToUnit(const QDateTime& baseDate, ChronoLineUnit unit)
{
    QDateTime dFloor = truncToUnit(baseDate, unit);
    if (fabs(unitsTo(dFloor, baseDate, unit))>1e-6)
        return addUnits(dFloor, 1, unit);
    else
        return dFloor;
}

// D/t round date to actual unit (to next, if >=0.5)
QDateTime CLTimeLine::roundToUnit(const QDateTime& baseDate, ChronoLineUnit unit)
{
    if (unit==cluAuto) unit = _actualUnit;
    QDateTime dFloor = truncToUnit(baseDate, unit);
    QDateTime dCeil = addUnits(dFloor, 1, unit);
    if (unitsTo(dFloor, baseDate, unit)>=0.5*unitsTo(dFloor, dCeil, unit))
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

// check if date range is correct
bool CLTimeLine::checkDateRange(const QDateTime &minDate, const QDateTime &maxDate)
{
    if (unitsTo(minDate, maxDate)<MIN_UNITS_NUM) return false;
    if (minDate.daysTo(maxDate)>MAX_YEARS_NUM*366) return false;
    return true;
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
    float delta = (float)(oldDragX-newDragX)/mainDivStep;
    setRange(addUnits(minDate(), delta), addUnits(maxDate(), delta), true);
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

