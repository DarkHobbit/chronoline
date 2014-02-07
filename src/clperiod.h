#ifndef CLPERIOD_H
#define CLPERIOD_H

#include <QDateTime>
#include <QGraphicsItem>
#include "cltimeline.h"

class CLPeriod : public QGraphicsItem
{
public:
    CLPeriod(long id, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color, CLTimeLine* timeLine);
    void paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget);
    virtual QRectF boundingRect() const;
    QDateTime minDate();
    QDateTime maxDate();
    void setMinDate(const QDateTime& minDate);
    void setMaxDate(const QDateTime& maxDate);
protected:
    // Input data
    QDateTime      _minDate, _maxDate;
    QColor _color;
    // Recalc flag
    bool changed;
    CLTimeLine* _timeLine;
    long _id;
};

#endif // CLPERIOD_H
