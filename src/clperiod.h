#ifndef CLPERIOD_H
#define CLPERIOD_H

#include <QDateTime>
#include <QGraphicsItem>
#include "cltimeline.h"

class CLPeriod : public QGraphicsItem
{
public:
    CLPeriod(long id, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color, CLTimeLine* timeLine);
    virtual ~CLPeriod();
    void paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget);
    virtual QRectF boundingRect() const;
    QDateTime minDate();
    QDateTime maxDate();
    void setMinDate(const QDateTime& minDate);
    void setMaxDate(const QDateTime& maxDate);
    long id();
    int level();
protected:
    // Input data
    QDateTime      _minDate, _maxDate;
    QColor _color;
    CLTimeLine* _timeLine;
    // Recalc flag
    bool changed;
    long _id;
};

#endif // CLPERIOD_H
