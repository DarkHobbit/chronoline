#ifndef CLFLAG_H
#define CLFLAG_H

#include <QDateTime>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "cltimeline.h"

class CLFlag : public QGraphicsItem
{
public:
    CLFlag(long id, const QDateTime& date, const ChronoLineFlagType& fType, const QColor& color, CLTimeLine* timeLine);
    void paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget);
    virtual QRectF boundingRect() const;
    QDateTime _date;
protected:
    // Input data
    ChronoLineFlagType _fType;
    QColor _color;
    // Recalc flag
    bool changed;
    CLTimeLine* _timeLine;
    long _id;
    int  dragBase; // position of cursor at start of drag
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CLFLAG_H
