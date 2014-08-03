#ifndef CLFLAG_H
#define CLFLAG_H

#include <QDateTime>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include "cltimeline.h"

enum FlagDragDirection {fdLeft, fdRight};

class CLFlag : public QGraphicsObject
{
    Q_OBJECT
public:
    CLFlag(long id, const QDateTime& date, const ChronoLineFlagType& fType, const QColor& color,
          CLTimeLine* timeLine, QObject* eventReceiver);
    void setPairFlag(CLFlag* pairFlag);
    bool setDate(const QDateTime& date, bool checkForPairDate = true);
    void setPosByDate(const QRect& r);
    void paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget);
    virtual QRectF boundingRect() const;
    QDateTime date();
    long id();
protected:
    // Input data
    QDateTime _date;
    ChronoLineFlagType _fType;
    QColor _color;
    CLFlag* _pairFlag;
    CLTimeLine* _timeLine;
    long _id;
    // Recalc flag
    bool changed;
    // Direction of flag for painting and bounding (depend of its type)
    int flagHeight, flagSubheight, flagWidth;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
    void draggedOutside(FlagDragDirection direction, int newX);
    void dragOutsideStop();
    void dateChanged(long idFlag, const QDateTime& newDate);
};

#endif // CLFLAG_H
