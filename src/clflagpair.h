#ifndef CLFLAGPAIR_H
#define CLFLAGPAIR_H

#include <QGraphicsItemGroup>
#include "clflag.h"
#include "cltimeline.h"

class CLFlagPair : public QGraphicsItemGroup
{
public:
    CLFlagPair(long id, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color, CLTimeLine* timeLine);
protected:
    CLFlag *begFlag, *endFlag;
    long _id;
};

#endif // CLFLAGPAIR_H
