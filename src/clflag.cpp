#include <iostream>
#include <QMessageBox>
#include <QPainter>
#include <QGraphicsScene>
#include "cldefs.h"
#include "clflag.h"

#include <QLabel>
extern QLabel* lbDebug;

CLFlag::CLFlag(long id, const QDateTime& date, const ChronoLineFlagType& fType, const QColor& color,
               CLTimeLine* timeLine, QObject* eventReceiver):
    _date(date),
    _fType(fType),
    _color(color),
    _timeLine(timeLine),
    _id(id),
    changed(false)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setPos(0, 1);
    // Signal handling
    connect(this,
        SIGNAL(draggedOutside(FlagDragDirection, int)),
        eventReceiver, SLOT(flagDraggedOutside(FlagDragDirection, int)));
    connect(this, SIGNAL(dragOutsideStop()), eventReceiver, SLOT(flagDragOutsideStop()));
    connect(this,
        SIGNAL(dateChanged(long, const QDateTime&)),
        eventReceiver, SLOT(transferFlagDateChanged(long, const QDateTime&)));
    // Direction of flag for painting and bounding (depend of its type)
    flagHeight = FLAG_HEIGHT;
    flagSubheight = FLAG_SUBHEIGHT;
    if (_fType!=clftEvent) {
        flagHeight = -flagHeight;
        flagSubheight = -flagSubheight;
    }
    flagWidth = -FLAG_WIDTH;
    if (_fType==clftPairEnd) flagWidth = -flagWidth;
}

CLFlag::~CLFlag()
{
    if (_timeLine->selectedObject==this)
        _timeLine->selectedObject = 0;
}

void CLFlag::setPairFlag(CLFlag* pairFlag)
{
    _pairFlag = pairFlag;
}

void CLFlag::paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    p->setPen(_color);
    p->drawLine(0, -1, 0, flagHeight);
    p->drawLine(0, flagHeight, flagWidth, flagHeight-flagSubheight/2);
    p->drawLine(flagWidth, flagHeight-flagSubheight/2, 0, flagHeight-flagSubheight);
}

QRectF CLFlag::boundingRect() const
{
    int x0 = flagWidth+1;
    int y0 = 2;
    if (_fType!=clftEvent) {
        if (_fType==clftPairEnd) x0 = 0;
        y0 = -FLAG_HEIGHT;
    }
    return QRectF(x0, y0, FLAG_WIDTH, FLAG_HEIGHT);
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
    // Check either flag dragged throw pair flag
    if (_fType!=clftEvent) {
        if ((_fType==clftPairBeg)&&(newX>=_pairFlag->pos().x())) return;
        if ((_fType==clftPairEnd)&&(newX<=_pairFlag->pos().x())) return;
    }
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

bool CLFlag::setDate(const QDateTime& date, bool checkForPairDate)
{
    if (checkForPairDate&&(_pairFlag!=0)) {
        if ((_fType==clftPairBeg)&&(date>=_pairFlag->date())) return false;
        if ((_fType==clftPairEnd)&&(date<=_pairFlag->date())) return false;
    }
    _date = date;
    update();
    return true;
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

