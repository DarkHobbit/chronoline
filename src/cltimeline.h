#ifndef CLTIMELINE_H
#define CLTIMELINE_H

#include <QDateTime>
#include <QGraphicsObject>
#include <QObject>
#include "cldefs.h"

class CLTimeLine : public QGraphicsObject
{
    Q_OBJECT
public:
    CLTimeLine();
    void paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget);
    virtual QRectF boundingRect() const;
    // Timeline settings
    void setUnit(const ChronoLineUnit& unit);
    void setMinDate(const QDateTime date);
    void setMaxDate(const QDateTime date);
    void zoomIn(float centerRate);
    void zoomOut(float centerRate);
    // Date/time coordinate (viewport x) for date
    int  xForDate(const QDateTime date, const QRect& r);
    QDateTime dateForX(int x);
    // Meazurement unit
    ChronoLineUnit unit();
    // if unit=auto, adjust real unit. Otherwise =unit
    ChronoLineUnit actualUnit();
    QDateTime minDate();
    QDateTime maxDate();
    int xMin();
    int xMax();
   // Rounded minDate for best view (read-only, calculated based on minDate)
    QDateTime leftScaleDate();
    // Calculate range, step, etc.
    bool calcScale(const QRect& r);
    // D/t length beetwen two dates in selected unit (daysTo() and secsTo()-like)
    float unitsTo(const QDateTime& baseDate, const QDateTime& newDate, const ChronoLineUnit unit);
    // D/t throw num units
    QDateTime addUnits(const QDateTime& baseDate, float num);
protected:
    // Input data
    ChronoLineUnit _unit, _actualUnit;
    QDateTime      _minDate, _maxDate;
    // Recalc flag
    bool changed;
    QRect rect;
    // Scale division parameters (calculated by CalcStep)
    int            mainDivCount;
    int            mainDivStep;
    int            x0;
    int            xN;
    QDateTime      _leftScaleDate;
    // Drag data
    int oldDragX;
    // draw date text
    void drawDate(QPainter *p, int x, const QDateTime& date, short level, ChronoLineUnit unit, bool forceDrawParent);
    // check if parent unit text draw needed
    bool parentTextNeeded(const QDateTime& d, ChronoLineUnit nextUnit);
    // mouse handling
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
    void needUpdateAll();
};

#endif // CLTIMELINE_H
