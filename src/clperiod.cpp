#include <iostream>
#include <QMessageBox>
#include <QPainter>
#include "cldefs.h"
#include "clperiod.h"

CLPeriod::CLPeriod(const QDateTime& minDate, const QDateTime& maxDate, CLTimeLine* timeLine):
    changed(false),
    _minDate(minDate),
    _maxDate(maxDate),
    _timeLine(timeLine)
{
}

void CLPeriod::paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
//    std::cout << "CLPeriod::paint" << std::endl;
}

QRectF CLPeriod::boundingRect() const
{
}

