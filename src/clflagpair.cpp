#include <QMessageBox>
#include "clflagpair.h"

CLFlagPair::CLFlagPair
(long id, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color,
 CLTimeLine* timeLine, QObject* eventReceiver):
_id(id)
{
    begFlag = new CLFlag(id+1, minDate, clftPairBeg, color, timeLine, eventReceiver);
    endFlag = new CLFlag(id+2, maxDate, clftPairEnd, color, timeLine, eventReceiver);
    begFlag->setParentItem(timeLine);
    endFlag->setParentItem(timeLine);
    begFlag->setPairFlag(endFlag);
    endFlag->setPairFlag(begFlag);
}

void CLFlagPair::setPosByDates(const QRect& r)
{
    begFlag->setPosByDate(r);
    endFlag->setPosByDate(r);
}

QDateTime CLFlagPair::minDate() { return begFlag->date(); }
QDateTime CLFlagPair::maxDate() { return endFlag->date(); }
void CLFlagPair::setMinDate(const QDateTime& minDate) { begFlag->setDate(minDate); }
void CLFlagPair::setMaxDate(const QDateTime& maxDate) { endFlag->setDate(maxDate); }
