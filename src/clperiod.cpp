#include <iostream>
#include <QMessageBox>
#include <QPainter>
#include "cldefs.h"
#include "clperiod.h"

CLPeriod::CLPeriod(long id, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color, CLTimeLine* timeLine):
    _id(id),
    changed(false),
    _minDate(minDate),
    _maxDate(maxDate),
    _color(color),
    _timeLine(timeLine)
{
}

void CLPeriod::paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    QRect v = p->viewport();
    int xBeg = _timeLine->xForDate(_minDate, v);
    int xEnd = _timeLine->xForDate(_maxDate, v);
    int level = _id-1; // TODO need optimize level (don't keep id wholes, one level for some non-crossing period, etc)
    int height = BASE_PERIOD_HEIGHT +level*PERIOD_HEIGHT_SHIFT;
    p->setPen(_color);
    p->drawLine(xBeg, 0, xBeg, -height);
    p->drawLine(xBeg, -height, xEnd, -height);
    p->drawLine(xEnd, -height, xEnd, 0);
//    std::cout << "CLPeriod::paint" << std::endl;
}

QRectF CLPeriod::boundingRect() const
{
}

