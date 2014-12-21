#ifndef CLFLAGPAIR_H
#define CLFLAGPAIR_H

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
    long id();
    virtual bool matchDate(const QDateTime& d);
    ChronoLineFlagType matchedFlag(const QDateTime& d);
protected:
    CLFlag *begFlag, *endFlag;
    long _id;
};

#endif // CLFLAGPAIR_H
