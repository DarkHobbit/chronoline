#include <iostream>
#include <QMessageBox>
#include <QPainter>
#include <QGraphicsScene>
#include "cldefs.h"
#include "clflag.h"

#include <QLabel>
extern QLabel* lbDebug;

CLFlag::CLFlag(long id, const QDateTime& date, const ChronoLineFlagType& fType, const QColor& color, CLTimeLine* timeLine):
    _id(id),
    _date(date),
    _fType(fType),
    _color(color),
    _timeLine(timeLine),
    changed(false)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setPos(0, 1);
}

void CLFlag::paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    // Direction of flag (depend of its type)
    int height = FLAG_HEIGHT;
    int subheight = FLAG_SUBHEIGHT;
    if (_fType!=clftEvent) {
        height = -height;
        subheight = -subheight;
    }
    int dX = -FLAG_WIDTH;
    if (_fType==clftPairEnd) dX = -dX;
    // Paint!
    p->setPen(_color);
    p->drawLine(0, -1, 0, height);
    p->drawLine(0, height, dX, height-subheight/2);
    p->drawLine(dX, height-subheight/2, 0, height-subheight);
}

QRectF CLFlag::boundingRect() const
{
    return QRectF(-FLAG_WIDTH+1, 2, FLAG_WIDTH, FLAG_HEIGHT);
}

void CLFlag::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void CLFlag::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Set new position
    int newX = event->scenePos().x(); // dragBase + pos().x() works wrong
    QDateTime newDate = _timeLine->dateForX(newX);
    // Debug coord output
    //lbDebug->setText(QString("%1 %2 %3").arg(scenePos().x()).arg(event->scenePos().x()).arg(_date.toString()));
    // Check either flag dragged left outside scale
    if (newDate<_timeLine->minDate())
        emit draggedOutside(fdLeft, newX);
    // Check either flag dragged right outside scale
    else if (newDate>_timeLine->maxDate())
        emit draggedOutside(fdRight, newX);
    // Move flag
    else {
        emit dragOutsideStop();
        _date = newDate;
        setPos(newX, 1);
        emit dateChanged(_id, newDate);
        scene()->update();
    }
    return;
}

void CLFlag::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    emit dragOutsideStop();
}

void CLFlag::setDate(const QDateTime& date)
{
    _date = date;
    update();
}

void CLFlag::setPosByDate(const QRect& r)
{
    setPos(_timeLine->xForDate(_date, r), 1);
}

QDateTime CLFlag::date()
{
    return _date;
}

long CLFlag::id()
{
    return _id;
}

