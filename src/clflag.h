#ifndef CLFLAG_H
#define CLFLAG_H

#include <QDateTime>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include "clselectableobject.h"

enum FlagDragDirection {fdLeft, fdRight};

class CLFlag : public QGraphicsObject, public CLSelectableObject
{
    Q_OBJECT
public:
    CLFlag(long id, const QDateTime& date, const ChronoLineFlagType& fType, const QColor& color,
          CLTimeLine* timeLine, QObject* eventReceiver, CLSelectableObject* pair);
    virtual ~CLFlag();
    void setPairFlag(CLFlag* pairFlag);
    bool setDate(const QDateTime& date, bool checkForPairDate = true);
    void setPosByDate(const QRect& r);
    void paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*);
    virtual QRectF boundingRect() const;
    QDateTime date();
    long id();
    ChronoLineFlagType fType();
    CLSelectableObject* pair();
    virtual bool matchDate(const QDateTime& d);
protected:
    // Input data
    long _id;
    QDateTime _date;
    ChronoLineFlagType _fType;
    QColor _color;
    CLFlag* _pairFlag;
    CLSelectableObject* _pair;
    // Recalc flag
    bool changed;
    // Direction of flag for painting and bounding (depend of its type)
    int flagHeight, flagSubheight, flagWidth;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
public: // need because manuall call from other flag
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
    void draggedOutside(FlagDragDirection direction, int newX);
    void dragOutsideStop();
    void dateChanged(const QDateTime& newDate);
};

#endif // CLFLAG_H
