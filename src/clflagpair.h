#ifndef CLFLAGPAIR_H
#define CLFLAGPAIR_H

#include <QColor>
#include <QGraphicsItemGroup>
#include "clflag.h"
#include "cltimeline.h"

class CLFlagPair : public QGraphicsItemGroup, public CLSelectableObject
{
public:
    CLFlagPair(long id, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color,
               CLTimeLine* timeLine, QObject* eventReceiver);
    virtual ~CLFlagPair();
    void setPosByDates(const QRect& r);
    QDateTime minDate();
    QDateTime maxDate();
    void setMinDate(const QDateTime& minDate);
    void setMaxDate(const QDateTime& maxDate);
    void setColor(const QColor& color);
    long id();
    virtual bool matchDate(const QDateTime& d);
    ChronoLineFlagType matchedFlag(const QDateTime& d);
public: // need because manuall call from other flag
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
protected:
    CLFlag *begFlag, *endFlag;
    long _id;
    bool begFlagPossiblySelected; // defined only if flag pair selected
};

#endif // CLFLAGPAIR_H
