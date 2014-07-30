#include "clflagpair.h"

CLFlagPair::CLFlagPair
(long id, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color, CLTimeLine* timeLine):
_id(id)
{
    begFlag = new CLFlag(id+1, minDate, clftPairBeg, color, timeLine);
    endFlag = new CLFlag(id+1, maxDate, clftPairEnd, color, timeLine);
}
