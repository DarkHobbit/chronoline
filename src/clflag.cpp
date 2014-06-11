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
    int height = FLAG_HEIGHT;
    int dX = -FLAG_WIDTH;
    if (_fType==clftPairEnd) dX = -dX;
    p->setPen(_color);
    p->drawLine(0, 1, 0, FLAG_HEIGHT);
    p->drawLine(0, FLAG_HEIGHT, dX, FLAG_HEIGHT-FLAG_SUBHEIGHT/2);
    p->drawLine(dX, FLAG_HEIGHT-FLAG_SUBHEIGHT/2, 0, FLAG_HEIGHT-FLAG_SUBHEIGHT);
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
        _date = newDate;
        setPos(newX, 1);
        scene()->update();
    }
    return;
}

void CLFlag::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    mouseMoveEvent(event);
}

void CLFlag::setDate(const QDateTime date)
{
    _date = date;
    update();
}

QDateTime CLFlag::date()
{
    return _date;
}

