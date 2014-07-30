#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsWidget>
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
    /*p->setPen(Qt::blue);
    int vh = v.height();
    p->drawLine(-vw/2+LEFT_DIV_MARGIN, -vh/2, -vw/2+LEFT_DIV_MARGIN, vh/2+9);
    p->drawLine(0, -vh/2, 0, vh/2+9);
    p->drawLine(vw/2-RIGHT_DIV_MARGIN, -vh/2, vw/2-RIGHT_DIV_MARGIN, vh/2+9);*/
    p->setPen(Qt::black);
    // Main scale divisions;
    int xPix = xForDate(_leftScaleDate, v) - mainDivStep;
    QDateTime xDate = _leftScaleDate; // TODO implement dateForX instead this g-code
    for (int i=0; i<(mainDivCount+2); i++) {
        // Division mark
        p->drawLine(xPix, 0, xPix, -MAIN_DIV_HEIGHT);
        // Division text
        p->drawText(xPix, TEXT_Y, xDate.toString(dateFormat));
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
            auxDivCount = 12; // TODO g-code end
        if (mainDivStep/auxDivCount>2) // prevent merging neighbor divisions
        for (int j=0; j<auxDivCount; j++) {
            int auxDivStep = j*mainDivStep/auxDivCount;
            p->drawLine(xPix+auxDivStep, 0, xPix+auxDivStep, -AUX_DIV_HEIGHT);
        }
        // To next division...
        xPix += mainDivStep;
        xDate = addUnits(xDate, 1);
    };
}

QRectF CLTimeLine::boundingRect() const
{
    return QRectF(-rect.width()/2, -rect.height()/2, rect.width(), rect.height());
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
    float unitsCount = unitsTo(_minDate, date, _actualUnit);
//std::cout << " uC=" << unitsCount << " lsd=" << leftScaleDate.toString().toLocal8Bit().data() << " dt=" << date.toString().toLocal8Bit().data() << std::endl;
    /*if (_actualUnit!=cluMonth)*/
        return x0+mainDivStep*unitsCount;
    /*else { // because 28, 29,30,31 days in month
        x0+=mainDivStep*(int)unitsCount;
        int dimCount = date.date().daysInMonth();
        int auxDivStep = mainDivStep/dimCount;
        float daysCount = (unitsCount-(int)unitsCount)*
        // TODO
        return x0+
    };*/
}

QDateTime CLTimeLine::dateForX(int x)
{
    QDateTime d = addUnits(_minDate, ((float)x-x0)/mainDivStep);
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
    rect = r;
    if (_minDate>=_maxDate) return false;
    actualUnit();
    if (_actualUnit==cluHour) {
        _leftScaleDate = QDateTime(_minDate.date(), QTime(_minDate.time().hour(), 0, 0));
        dateFormat = "hh:mm";
    } else
    if (_actualUnit==cluDay) {
        _leftScaleDate = QDateTime(_minDate.date());
        dateFormat = "dd.MM";
    } else
    if (_actualUnit==cluWeek) {
        _leftScaleDate = _minDate; // TODO adjust to nice scale
        dateFormat = "dd.MM";
    } else
    if (_actualUnit==cluMonth) {
        _leftScaleDate = QDateTime(QDate(_minDate.date().year(), _minDate.date().month(), 1));
        dateFormat = "MMM";
    } else
    if (_actualUnit==cluQuarter) {
        _leftScaleDate = _minDate; // TODO adjust to nice scale
        dateFormat = "MMM";
    } else {
        _leftScaleDate = QDateTime(QDate(_minDate.date().year(), 1, 1));
        dateFormat = "yyyy";
    };
    mainDivCount = (int)unitsTo(_leftScaleDate, _maxDate, _actualUnit)+1;
    if (mainDivCount<2) return false;
    mainDivStep = (r.width()-LEFT_DIV_MARGIN-RIGHT_DIV_MARGIN) / (mainDivCount-1);
    x0 = -r.width()/2+LEFT_DIV_MARGIN;
    xN = r.width()/2-RIGHT_DIV_MARGIN;
    changed = false;
    return true;
}

// D/t length beetwen two dates in selected unit (daysTo() and secsTo()-like)
float CLTimeLine::unitsTo(const QDateTime& baseDate, const QDateTime& newDate, const ChronoLineUnit unit)
{
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

// D/t throw num units
QDateTime CLTimeLine::addUnits(const QDateTime& baseDate, float num)
{
    if (_actualUnit==cluHour)
        return baseDate.addSecs(num*3600);
    else if (_actualUnit==cluDay)
        return baseDate.addSecs(num*3600*24);
    else if (_actualUnit==cluWeek)
        return baseDate.addDays(num*7);
    else if (_actualUnit==cluMonth) // TODO: month and above - variable unit step
        return baseDate.addDays(num*30);
    else if (_actualUnit==cluQuarter)
        return baseDate.addDays(num*90);
    else
        return baseDate.addDays(num*365);
}

QString d2c(const QDateTime& d) { return d.toString("dd.MM.yyyy hh:mm:ss"); }

void CLTimeLine::zoomIn(float centerRate)
{
    setMinDate(addUnits(minDate(), centerRate*ZOOM_STEP));
    setMaxDate(addUnits(maxDate(), -(1-centerRate)*ZOOM_STEP));
}

void CLTimeLine::zoomOut(float centerRate)
{
    setMinDate(addUnits(minDate(), -centerRate*ZOOM_STEP));
    setMaxDate(addUnits(maxDate(), (1-centerRate)*ZOOM_STEP));
}

void CLTimeLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    oldDragX = event->scenePos().x();
//QMessageBox::information(0, "debug", QString::number(oldDragX));
    prepareGeometryChange(); // ?
    update();
}

void CLTimeLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Set new position
    int newDragX = event->scenePos().x();
    // Recalc dates
    setMinDate(addUnits(minDate(), (float)(oldDragX-newDragX)/mainDivStep));
    setMaxDate(addUnits(maxDate(), (float)(oldDragX-newDragX)/mainDivStep));
    // preparing for next move
    oldDragX = newDragX;
    update();
}

void CLTimeLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

