#include <iostream>
#include <QMessageBox>
#include <QPainter>
#include "cldefs.h"
#include "clperiod.h"

CLPeriod::CLPeriod(long id, const QDateTime& minDate, const QDateTime& maxDate, CLTimeLine* timeLine):
    _id(id),
    changed(false),
    _minDate(minDate),
    _maxDate(maxDate),
    _timeLine(timeLine)
{
}

void CLPeriod::paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    QRect v = p->viewport();
    int xBeg = _timeLine->xForDate(_minDate, v);
    int xEnd = _timeLine->xForDate(_maxDate, v);
    int height = BASE_PERIOD_HEIGHT; // TODO +level*PERIOD_HEIGHT_SHIFT
    p->drawLine(xBeg, 0, xBeg, -height);
    p->drawLine(xBeg, -height, xEnd, -height);
    p->drawLine(xEnd, -height, xEnd, 0);
//    std::cout << "CLPeriod::paint" << std::endl;
}

QRectF CLPeriod::boundingRect() const
{
}

