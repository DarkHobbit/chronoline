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
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptsHoverEvents(true);
}

void CLFlag::paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    QRect v = p->viewport();
    int x = _timeLine->xForDate(_date, v);
    int height = FLAG_HEIGHT;
    int dX = -FLAG_WIDTH;
    if (_fType==clftPairEnd) dX = -dX;
    p->setPen(_color);
    p->drawLine(x, 1, x, FLAG_HEIGHT);
    p->drawLine(x, FLAG_HEIGHT, x+dX, FLAG_HEIGHT-FLAG_SUBHEIGHT/2);
    p->drawLine(x+dX, FLAG_HEIGHT-FLAG_SUBHEIGHT/2, x, FLAG_HEIGHT-FLAG_SUBHEIGHT);
//    std::cout << "CLFlag::paint" << std::endl;
}

QRectF CLFlag::boundingRect() const
{
    return QRectF(-1, 1, FLAG_WIDTH, FLAG_HEIGHT);
}

void CLFlag::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void CLFlag::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
/*    if (event->buttons() & Qt::LeftButton) {
        moveBy(event->pos().x(), 0);
        parentItem()->update();
        update();
        event->accept();
    }
    else
        event->ignore();*/
std::cout << event->pos().x() << std::endl;

    if (event->modifiers() & Qt::ShiftModifier) {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void CLFlag::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
/*    QGraphicsItem::mouseReleaseEvent(event);
    update();*/
/*        parentItem()->update();
        update();
        event->accept();*/
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
