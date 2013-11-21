#ifndef CLTIMELINE_H
#define CLTIMELINE_H

#include <QDateTime>
#include <QGraphicsItem>

class CLTimeLine : public QGraphicsItem
{
public:
    CLTimeLine();
    void paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget);
    virtual QRectF boundingRect() const;
    // Timeline settings
    void setUnit(const ChronoLineUnit& unit);
    void setMinDate(const QDateTime date);
    void setMaxDate(const QDateTime date);
    // Date/time coordinate (viewport x) for date
    int  xForDate(const QDateTime date, const QRect& r);
    // Meazurement unit
    ChronoLineUnit unit();
    // if unit=auto, adjust real unit. Otherwise =unit
    ChronoLineUnit actualUnit();
    QDateTime minDate();
    QDateTime maxDate();
    // Calculate range, step, etc.
    bool calcScale(const QRect& r);
protected:
    // Input data
    ChronoLineUnit _unit, _actualUnit;
    QDateTime      _minDate, _maxDate;
    // Recalc flag
    bool changed;
    // Scale division parameters (calculated by CalcStep)
    int            mainDivCount;
    int            mainDivStep;
    QDateTime      leftScaleDate;
    QString        dateFormat;
};

#endif // CLTIMELINE_H
