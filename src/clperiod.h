#ifndef CLPERIOD_H
#define CLPERIOD_H

#include <QDateTime>
#include <QGraphicsItem>
#include "cltimeline.h"

class CLPeriod : public QGraphicsItem
{
public:
    CLPeriod(const QDateTime& minDate, const QDateTime& maxDate, CLTimeLine* timeLine);
    void paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget);
    virtual QRectF boundingRect() const;
protected:
    // Input data
    QDateTime      _minDate, _maxDate;
    // Recalc flag
    bool changed;
    CLTimeLine* _timeLine;
};

#endif // CLPERIOD_H
