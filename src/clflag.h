#ifndef CLFLAG_H
#define CLFLAG_H

#include <QDateTime>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include "cltimeline.h"

class CLFlag : public QGraphicsObject
{
    Q_OBJECT
public:
    enum DragDirection {fdLeft, fdRight};
    CLFlag(long id, const QDateTime& date, const ChronoLineFlagType& fType, const QColor& color, CLTimeLine* timeLine);
    void paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget);
    virtual QRectF boundingRect() const;
    void setDate(const QDateTime date);
    QDateTime date();
protected:
    // Input data
    QDateTime _date;
    ChronoLineFlagType _fType;
    QColor _color;
    // Recalc flag
    bool changed;
    CLTimeLine* _timeLine;
    long _id;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
    void draggedOutside(DragDirection direction, int newX, const QDateTime& newDate);
};

#endif // CLFLAG_H
