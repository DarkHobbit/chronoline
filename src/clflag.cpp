#include <iostream>
#include <QMessageBox>
#include <QPainter>
#include "cldefs.h"
#include "clflag.h"

CLFlag::CLFlag(long id, const QDateTime& date, const ChronoLineFlagType& fType, const QColor& color, CLTimeLine* timeLine):
    _id(id),
    changed(false),
    _date(date),
    _fType(fType),
    _color(color),
    _timeLine(timeLine)
{
}

void CLFlag::paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    QRect v = p->viewport();
    int x = _timeLine->xForDate(_date, v);
    int height = FLAG_HEIGHT;
    int dX = -FLAG_WIDTH;
    if (_fType==clftPairEnd) dX = -dX;
    p->setPen(_color);
    p->drawLine(x, 0, x, FLAG_HEIGHT);
    p->drawLine(x, FLAG_HEIGHT, x+dX, FLAG_HEIGHT-FLAG_SUBHEIGHT/2);
    p->drawLine(x+dX, FLAG_HEIGHT-FLAG_SUBHEIGHT/2, x, FLAG_HEIGHT-FLAG_SUBHEIGHT);
//    std::cout << "CLFlag::paint" << std::endl;
}

QRectF CLFlag::boundingRect() const
{
}

