#include <QMessageBox>
#include "clflagpair.h"

CLFlagPair::CLFlagPair
(long id, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color,
 CLTimeLine* timeLine, QObject* eventReceiver):
    CLSelectableObject(timeLine),
    _id(id)
{
    begFlag = new CLFlag(id+1, minDate, clftPairBeg, color, timeLine, eventReceiver, this);
    endFlag = new CLFlag(id+2, maxDate, clftPairEnd, color, timeLine, eventReceiver, this);
    begFlag->setParentItem(timeLine);
    endFlag->setParentItem(timeLine);
    begFlag->setPairFlag(endFlag);
    endFlag->setPairFlag(begFlag);
}

CLFlagPair::~CLFlagPair()
{
    if (_timeLine->selectedObject==this)
        _timeLine->selectedObject = 0;
    delete begFlag;
    delete endFlag;
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

long CLFlagPair::id()
{
    return _id;
}

bool CLFlagPair::matchDate(const QDateTime& d)
{
    begFlagPossiblySelected = begFlag->matchDate(d);
    return (begFlag->matchDate(d)||endFlag->matchDate(d));
}

ChronoLineFlagType CLFlagPair::matchedFlag(const QDateTime& d)
{
    if (begFlag->matchDate(d))
        return clftPairBeg;
    else return clftPairEnd;
}

void CLFlagPair::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (begFlagPossiblySelected)
        begFlag->mouseMoveEvent(event);
    else
        endFlag->mouseMoveEvent(event);
}

void CLFlagPair::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (begFlagPossiblySelected)
        begFlag->mouseReleaseEvent(event);
    else
        endFlag->mouseReleaseEvent(event);
}
