#include <iostream>
#include <QMessageBox>
#include <QPainter>
#include "cldefs.h"
#include "clperiod.h"

CLPeriod::CLPeriod(long id, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color, CLTimeLine* timeLine):
    CLSelectableObject(timeLine),
    _minDate(minDate),
    _maxDate(maxDate),
    _color(color),
    changed(false),
    _id(id)
{
}

CLPeriod::~CLPeriod()
{
    if (_timeLine->selectedObject==this)
        _timeLine->selectedObject = 0;
}

int CLPeriod::level()
{
    return _id-1; // TODO need optimize level (don't keep id wholes, one level for some non-crossing period, etc)
}

void CLPeriod::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
{
    QRect v = p->viewport();
    int xBeg = _timeLine->xForDate(_minDate, v);
    int xEnd = _timeLine->xForDate(_maxDate, v);
    int height = BASE_PERIOD_HEIGHT +level()*PERIOD_HEIGHT_SHIFT;
    QPen _pen;
    _pen.setColor(_color);
    setPenWidth(_pen);
    p->setPen(_pen);
    p->drawLine(xBeg, 0, xBeg, -height);
    p->drawLine(xBeg, -height, xEnd, -height);
    p->drawLine(xEnd, -height, xEnd, 0);
//    std::cout << "CLPeriod::paint" << std::endl;
}

QRectF CLPeriod::boundingRect() const
{
    return QRectF(0, 0, 1, 1);
}

QDateTime CLPeriod::minDate() { return _minDate; }
QDateTime CLPeriod::maxDate() { return _maxDate; }
void CLPeriod::setMinDate(const QDateTime& minDate) { _minDate = minDate; }
void CLPeriod::setMaxDate(const QDateTime& maxDate) { _maxDate = maxDate; }

void CLPeriod::setColor(const QColor &color)
{
    _color = color;
}

long CLPeriod::id() { return _id; }

bool CLPeriod::matchDate(const QDateTime& d)
{
    return ((minDate()<d)&&(maxDate()>d));
}

