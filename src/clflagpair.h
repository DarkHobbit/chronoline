#ifndef CLFLAGPAIR_H
#define CLFLAGPAIR_H

#include <QGraphicsItemGroup>
#include "clflag.h"
#include "cltimeline.h"

class CLFlagPair : public QGraphicsItemGroup
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
protected:
    CLFlag *begFlag, *endFlag;
    long _id;
    CLTimeLine* _timeLine;
};

#endif // CLFLAGPAIR_H
